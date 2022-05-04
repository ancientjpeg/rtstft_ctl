/*
  ==============================================================================

    RT_FileTree.h
    Created: 3 May 2022 8:03:36am
    Author:  Jackson Kaplan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class RT_FileTree {
public:
  RT_FileTree(juce::File inTreeRootFile, juce::String inFilePattern = "*",
              RT_FileTree *inParent = nullptr);
  ~RT_FileTree();
  RT_FileTree(RT_FileTree &&inFileTreeRvRef) = delete;
  RT_FileTree      &operator=(RT_FileTree &&inFileTreeRvRef);

  juce::File        getDirFileObject();
  juce::String      getDirName();
  RT_FileTree      *getParent();
  juce::StringArray getFileNames(bool sort = true);
  bool              traverseToDirectory(int inIndex);

private:
  RT_FileTree(juce::File inTreeRootFile, juce::String inFilePattern,
              RT_FileTree *inParent, bool inIsRoot);
  bool                     mIsRoot;
  juce::String             mFilePattern;
  juce::File               mFileObject;
  juce::Array<juce::File>  mFiles;
  juce::Array<RT_FileTree> mDirs;
  RT_FileTree             *mParent;
};