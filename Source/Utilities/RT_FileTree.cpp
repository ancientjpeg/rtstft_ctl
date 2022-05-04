/*
  ==============================================================================

    RT_FileTree.cpp
    Created: 3 May 2022 8:03:36am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#include "RT_FileTree.h"

//RT_FileTree::RT_FileTree(juce::File inTreeRootFile, juce::String inFilePattern,
//                         RT_FileTree *inParent)
//    : RT_FileTree(inTreeRootFile, inFilePattern, inParent, true)
//{
//}
//RT_FileTree::RT_FileTree(juce::File inTreeRootFile, juce::String inFilePattern,
//                         RT_FileTree *inParent, bool inIsRoot)
//    : mFileObject(inTreeRootFile), mFilePattern(inFilePattern),
//      mParent(inParent), mIsRoot(inIsRoot)
//{
//  if (mIsRoot) {
//  }
//  mFiles = mFileObject.findChildFiles(2, false, mFilePattern);
//  for (auto dir : mFileObject.findChildFiles(1, false))
//    // could be a bit more memory efficient but..
//    mDirs.add(RT_FileTree(dir, mFilePattern, this, false));
//}
//RT_FileTree::~RT_FileTree() {}
//RT_FileTree &RT_FileTree::operator=(RT_FileTree &&inFileTreeRvRef)
//{
//  assert(inFileTreeRvRef.mIsRoot);
//  mIsRoot      = inFileTreeRvRef.mIsRoot;
//  mFilePattern = inFileTreeRvRef.mFilePattern;
//  mFileObject  = inFileTreeRvRef.mFileObject;
//  mFiles       = std::move(inFileTreeRvRef.mFiles);
//  mDirs        = std::move(mDirs);
//}
//
//juce::File   RT_FileTree::getDirFileObject() { return mFileObject; }
//juce::String RT_FileTree::getDirName() { return mFileObject.getFileName(); }
//RT_FileTree *RT_FileTree::getParent() { return mParent; }
//
//juce::StringArray RT_FileTree::getFileNames(bool sort)
//{
//  juce::StringArray ret;
//  for (auto &dir : mDirs) {
//    ret.add(dir.getDirName());
//  }
//  for (auto &file : mFiles) {
//    ret.add(file.getFileName());
//  }
//  return ret;
//}
