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

  struct FileDescription {
    bool         isDir;
    juce::String fileName;
  };
  class FileDescriptionComparator {
  public:
    static int compareElements(FileDescription &f0, FileDescription &f1);
  };

  juce::File                   getDirFileObject();
  juce::String                 getDirName();
  juce::Array<FileDescription> getFileDescriptions(bool sort = true);
  bool                         traverseUp();
  bool                         traverseDown(juce::String inTraversalTargetDir);
  const juce::Array<juce::File> &getChildFileArray();

private:
  juce::File              mTreeRoot, mCurrentDir;
  juce::String            mFilePattern;
  juce::Array<juce::File> mFiles;
  RT_FileTree            *mParent;

  void                    _constructTree();
};
