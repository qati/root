/// \file
/// \ingroup tutorial_multicore
/// \notebook -nodraw
/// Illustrate the usage of the multiproc TSelector interfaces with the h1 analysis
/// example.
///
/// \macro_code
///
/// \author Anda Chelba
/// \author Gerardo Ganis

#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH1F.h"
#include "TTreeReader.h"
#include "TProcPool.h"

const char *fh1[] = {"http://root.cern.ch/files/h1/dstarmb.root",
                     "http://root.cern.ch/files/h1/dstarp1a.root",
                     "http://root.cern.ch/files/h1/dstarp1b.root",
                     "http://root.cern.ch/files/h1/dstarp2.root"};

int mp103_processSelector(){

  // MacOSX may generate connection to WindowServer errors
  gROOT->SetBatch(kTRUE);

  TString selectorPath = gROOT->GetTutorialsDir();
  selectorPath += "/tree/h1analysis.C+";
  std::cout << "selector used is: "<< selectorPath<<"\n";
  TSelector *sel = TSelector::GetSelector(selectorPath);

// The following code generates a crash when Davix is used for HTTP
// Davix does not seem fork-safe; the problem has been reported to the
// Davix developers. For the time being we disable this part.
// To repoduce the problem, uncomment the next line.
//
// #define __reproduce_davix
#if defined(__reproduce_davix)
  TFile *fp = TFile::Open(fh1[0]);
  TTree *tree = (TTree *) fp->Get("h42");
#endif

  TProcPool pool(3);

  TList* out = 0;
#if defined(__reproduce_davix)
  //TProcPool::Process with a single tree
  out = pool.ProcTree(*tree, *sel);;
  sel->GetOutputList()->Delete();
#endif

  //TProcPool::Process with single file name and tree name
  //Note: we have less files than workers here
  out = pool.ProcTree(fh1[0], *sel, "h1");
  sel->GetOutputList()->Delete();

  // Prepare datasets: vector of files, TFileCollection
  TFileCollection fc;
  std::vector<std::string> files;
  for (int i = 0; i < 4; i++) {
     files.push_back(fh1[i]);
     fc.Add(new TFileInfo(fh1[i]));
  }

  //TProcPool::Process with vector of files and tree name
  //Note: we have more files than workers here (different behaviour)
  out = pool.ProcTree(files, *sel, "h1");
  sel->GetOutputList()->Delete();

  //TProcPool::Process with TFileCollection, no tree name
  out = pool.ProcTree(fc, *sel);
  sel->GetOutputList()->Delete();

  return 0;
}
