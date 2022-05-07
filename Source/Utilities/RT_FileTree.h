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
  RT_FileTree(juce::File inTreeRoot, juce::String inFilePattern = "*");
  RT_FileTree(RT_FileTree &&inFileTreeRvRef) = delete;
  ~RT_FileTree();

  RT_FileTree &operator=(const RT_FileTree &inFileTreeRvRef);
  RT_FileTree &operator=(RT_FileTree &&inFileTreeRvRef);

  class FileComparator {
  public:
    static int compareElements(juce::File &f0, juce::File &f1);
  };

  juce::File   getDirFileObject();
  juce::String getDirName();
  bool         traverseUp();
  bool         traverseDown(juce::String inTraversalTargetDir);
  const juce::Array<juce::File> *const getChildFileArray();
  juce::StringArray getFilenames(bool inWithSuffixes = false);

private:
  juce::File              mTreeRoot, mCurrentDir;
  juce::String            mFilePattern;
  juce::Array<juce::File> mFiles;
  RT_FileTree            *mParent;

  void                    _constructTree();
};
