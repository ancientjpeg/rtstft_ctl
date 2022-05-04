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

const juce::Array<juce::File> *const RT_FileTree::getChildFileArray()
{
  return &mFiles;
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
  mFiles = mCurrentDir.findChildFiles(3 | 4, false, mFilePattern);
}

int RT_FileTree::FileComparator::compareElements(juce::File &f0, juce::File &f1)
{
  return f0.getFileName().compareNatural(f1.getFileName());
}