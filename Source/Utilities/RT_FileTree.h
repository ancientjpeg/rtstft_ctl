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
  RT_FileTree(juce::File inTreeRootFile);
  ~RT_FileTree();
  RT_FileTree(RT_FileTree &&InFileTreeRvRef);
  RT_FileTree &operator=(RT_FileTree &&InFileTreeRvRef);

  juce::File   getTreeRootDirObject();

private:
  void                     constructFileTree(juce::File inTreeRootFile);
  bool                     mIsRoot;
  juce::File               mNodeDirFileObject;
  juce::Array<juce::File>  mFilesInDir;
  juce::Array<RT_FileTree> mDirsInDir;
  RT_FileTree             *mParent = nullptr;
};