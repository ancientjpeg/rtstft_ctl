/*
  ==============================================================================

    RT_FileTree.cpp
    Created: 3 May 2022 8:03:36am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileTree.h"

RT_FileTree::RT_FileTree(juce::File inTreeRoot, juce::String inFilePattern)
    : mTreeRoot(inTreeRoot), mCurrentDir(mTreeRoot), mFilePattern(inFilePattern)
{
  _constructTree();
}
RT_FileTree::~RT_FileTree() {}
RT_FileTree &RT_FileTree::operator=(RT_FileTree &&inFileTreeRvRef)
{
  mTreeRoot    = inFileTreeRvRef.mTreeRoot;
  mCurrentDir  = inFileTreeRvRef.mCurrentDir;
  mFilePattern = inFileTreeRvRef.mFilePattern;
  mFiles       = std::move(inFileTreeRvRef.mFiles);
}

juce::File   RT_FileTree::getDirFileObject() { return mCurrentDir; }
juce::String RT_FileTree::getDirName() { return mCurrentDir.getFileName(); }

const juce::Array<juce::File> &RT_FileTree::getChildFileArray()
{
  return mFiles;
}

juce::Array<RT_FileTree::FileDescription>
RT_FileTree::getFileDescriptions(bool sort)
{
  juce::Array<FileDescription> ret;
  for (juce::File f : mFiles) {
    ret.add({f.isDirectory(), f.getFileName()});
  }
  if (sort) {
    FileDescriptionComparator sorter;
    ret.sort(sorter);
  }
}

bool RT_FileTree::traverseUp()
{
  if (mCurrentDir != mTreeRoot) {
    mCurrentDir = mCurrentDir.getParentDirectory();
    return true;
  }
  return false;
}
bool RT_FileTree::traverseDown(juce::String inTraversalTargetDir)
{
  juce::File targetDir(inTraversalTargetDir);
  if (targetDir.isDirectory()) {
    mCurrentDir = targetDir;
    _constructTree();
    return true;
  }
  return false;
}

void RT_FileTree::_constructTree()
{
  mFiles = mCurrentDir.findChildFiles(4, false, mFilePattern);
}

int RT_FileTree::FileDescriptionComparator::compareElements(
    RT_FileTree::FileDescription &f0, RT_FileTree::FileDescription &f1)
{
  return f0.fileName.compareNatural(f1.fileName);
}