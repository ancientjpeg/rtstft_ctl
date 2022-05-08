/*
  ==============================================================================

    RT_FileTree.cpp
    Created: 3 May 2022 8:03:36am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileTree.h"
#include "../Managers/DSPStateManagers/RTSTFT_Manager.h"

RT_FileTree::RT_FileTree(juce::File inTreeRoot, juce::String inTargetSuffix)
    : mTreeRoot(inTreeRoot), mTargetSuffix(inTargetSuffix),
      mFilePattern("*" + mTargetSuffix)
{
  traverseDown(mTreeRoot);
}
RT_FileTree::~RT_FileTree() {}
RT_FileTree &RT_FileTree::operator=(RT_FileTree &&inFileTreeRvRef)
{
  mTreeRoot     = inFileTreeRvRef.mTreeRoot;
  mTargetSuffix = inFileTreeRvRef.mTargetSuffix;
  mFilePattern  = inFileTreeRvRef.mFilePattern;
  mDirStack     = std::move(inFileTreeRvRef.mDirStack);
}

/**
 * @brief pops the deepest Directory object from the Tree stack
 *
 * @return True if the operation succeeded, else false.
 */
bool RT_FileTree::traverseUp()
{
  if (mCurrentDir != mTreeRoot && mDirStack.size() > 1) {
    mDirStack.removeLast();
    return true;
  }
  return false;
}

/**
 * @brief Pushes the a new Directory object onto the Tree stack.
 *
 * @param inTargetDir The directory you'd like to push to the stack. Must be a
 * child of the current top of the stack.
 * @return Pointer to the newly pushed Directory object, or nullptr on failure.
 */
RT_FileTree::Directory *RT_FileTree::traverseDown(juce::File inTargetDir)
{
  if (mDirStack.size() < 1
      || (inTargetDir.isDirectory()
          && inTargetDir.getParentDirectory()
                 == mDirStack.getLast()->getDir())) {
    return mDirStack.add(new Directory(inTargetDir, this));
  }
  return nullptr;
}

int RT_FileTree::FileComparator::compareElements(juce::File &f0, juce::File &f1)
{
  return f0.getFileName().compareNatural(f1.getFileName());
}

juce::Array<juce::File>
RT_FileTree::getObjectsForAllFilesRecursive(bool inIncludeDirs)
{
  juce::Array<juce::File> ret;
  for (auto f : mTreeRoot.findChildFiles(juce::File::findFilesAndDirectories
                                             | juce::File::ignoreHiddenFiles,
                                         true, mFilePattern)) {
    if (!f.isDirectory() || inIncludeDirs) {
      ret.add(f);
    }
  }
  return ret;
}

/******************************************************************************/
/*******************       RT_FileTree::Directory          *******************/
/******************************************************************************/

RT_FileTree::Directory::Directory(juce::File inDir, RT_FileTree *inParentTree)
    : mDir(inDir), mParentTree(inParentTree),
      mChildFiles(mDir.findChildFiles(6, false, mParentTree->mFilePattern))
{
  FileComparator c;
  mChildFiles.addArray(mDir.findChildFiles(5, false));
  mChildFiles.sort(c, false);
}

RT_FileTree::Directory &
RT_FileTree::Directory::operator=(Directory &&inDirToMove) noexcept
{
  mDir        = inDirToMove.mDir;
  mParentTree = inDirToMove.mParentTree;
  mChildFiles = std::move(inDirToMove.mChildFiles);
}

const juce::Array<juce::File> const RT_FileTree::Directory::getChildFileArray()
{
  const juce::Array<juce::File> ret = mChildFiles;
  return ret;
}
const juce::Array<juce::File> *
RT_FileTree::Directory::getChildFileArrayPtr() const
{
  return &mChildFiles;
}
juce::File RT_FileTree::Directory::getFileByBasename(juce::String inBasename)
{
  int        depth = rt_log2_floor(mChildFiles.size());
  int        pos   = depth / 2, radius, comp;
  juce::File curr;
  while (depth-- > 0) {
    radius = (1 << (depth - 1));
    curr   = mChildFiles[pos];
    comp   = inBasename.compareNatural(curr.getFileNameWithoutExtension());
    if (comp == 0) {
      return curr;
    }
    else if (comp < 0) {
      pos -= radius;
    }
    else {
      pos += radius;
    }
  }
}
juce::File RT_FileTree::Directory::getDir() { return mDir; }
