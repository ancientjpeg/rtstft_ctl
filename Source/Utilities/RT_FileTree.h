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
  RT_FileTree(juce::File inTreeRoot, juce::String inTargetSuffix = "");
  RT_FileTree(RT_FileTree &&inFileTreeRvRef) = delete;
  ~RT_FileTree();

  RT_FileTree &operator=(const RT_FileTree &inFileTreeRvRef);
  RT_FileTree &operator=(RT_FileTree &&inFileTreeRvRef);

  using File = std::pair<juce::String, juce::File>;
  class Directory {
  public:
    Directory(juce::File inDir, RT_FileTree *inParentTree);
    Directory &operator=(Directory &&inDirToMove) noexcept;
    Directory &operator=(const Directory &) = delete;
    const juce::Array<juce::File> const getChildFileArray();
    const juce::Array<juce::File>      *getChildFileArrayPtr() const;
    juce::File                          getDir();
    juce::File getFileByBasename(juce::String inBasename);

  private:
    juce::File              mDir;
    RT_FileTree            *mParentTree;
    juce::Array<juce::File> mChildFiles;
  };

  class FileComparator {
  public:
    static int compareElements(juce::File &f0, juce::File &f1);
  };

  bool                    traverseUp();
  Directory              *traverseDown(juce::File inTargetDir);
  juce::StringArray       getFilenames(bool inWithSuffixes = false);
  juce::Array<juce::File> getObjectsForAllFilesRecursive(bool inIncludeDirs
                                                         = false);

private:
  juce::File                  mTreeRoot, mCurrentDir;
  juce::String                mTargetSuffix, mFilePattern;
  juce::OwnedArray<Directory> mDirStack;
};
