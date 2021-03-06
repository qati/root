// @(#)root/tmva $Id$
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer, Helge Voss, Kai Voss, Eckhard von Toerne, Jan Therhaag, Omar Zapata, Lorenzo Moneta, Sergei Gleyzer
//NOTE: Based on TMVA::Factory

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : DataLoader                                                            *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      This is a class to load datasets into every booked method                 *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Lorenzo Moneta <Lorenzo.Moneta@cern.ch> - CERN, Switzerland               *
 *      Omar Zapata <andresete.chaos@gmail.com>  - ITM/UdeA, Colombia             *
 *      Sergei Gleyzer<sergei.gleyzer@cern.ch> - CERN, Switzerland                *
 *                                                                                *
 * Copyright (c) 2005-2011:                                                       *
 *      CERN, Switzerland                                                         *
 *      ITM/UdeA, Colombia                                                        *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_DataLoader
#define ROOT_TMVA_DataLoader


#include <string>
#include <vector>
#include <map>
#ifndef ROOT_TCut
#include "TCut.h"
#endif

#ifndef ROOT_TMVA_Factory
#include "TMVA/Factory.h"
#endif
#ifndef ROOT_TMVA_Types
#include "TMVA/Types.h"
#endif
#ifndef ROOT_TMVA_DataSet
#include "TMVA/DataSet.h"
#endif

class TFile;
class TTree;
class TDirectory;

namespace TMVA {

   class IMethod;
   class MethodBase;
   class DataInputHandler;
   class DataSetInfo;
   class DataSetManager;
   class VariableTransformBase;

   class DataLoader : public Configurable {
      friend class Factory;
   public:

       DataLoader( TString thedlName="default");

      // default destructor
      virtual ~DataLoader();


      // add events to training and testing trees
      void AddSignalTrainingEvent    ( const std::vector<Double_t>& event, Double_t weight = 1.0 );
      void AddBackgroundTrainingEvent( const std::vector<Double_t>& event, Double_t weight = 1.0 );
      void AddSignalTestEvent        ( const std::vector<Double_t>& event, Double_t weight = 1.0 );
      void AddBackgroundTestEvent    ( const std::vector<Double_t>& event, Double_t weight = 1.0 );
      void AddTrainingEvent( const TString& className, const std::vector<Double_t>& event, Double_t weight );
      void AddTestEvent    ( const TString& className, const std::vector<Double_t>& event, Double_t weight );
      void AddEvent        ( const TString& className, Types::ETreeType tt, const std::vector<Double_t>& event, Double_t weight );
      Bool_t UserAssignEvents(UInt_t clIndex);
      TTree* CreateEventAssignTrees( const TString& name );

      DataSetInfo& AddDataSet( DataSetInfo& );
      DataSetInfo& AddDataSet( const TString&  );

      // special case: signal/background

      // Data input related
      void SetInputTrees( const TString& signalFileName, const TString& backgroundFileName, 
                          Double_t signalWeight=1.0, Double_t backgroundWeight=1.0 );
      void SetInputTrees( TTree* inputTree, const TCut& SigCut, const TCut& BgCut );
      // Set input trees  at once
      void SetInputTrees( TTree* signal, TTree* background, 
                          Double_t signalWeight=1.0, Double_t backgroundWeight=1.0) ;

      void AddSignalTree( TTree* signal,    Double_t weight=1.0, Types::ETreeType treetype = Types::kMaxTreeType );
      void AddSignalTree( TString datFileS, Double_t weight=1.0, Types::ETreeType treetype = Types::kMaxTreeType );
      void AddSignalTree( TTree* signal, Double_t weight, const TString& treetype );      

      // ... depreciated, kept for backwards compatibility
      void SetSignalTree( TTree* signal, Double_t weight=1.0);

      void AddBackgroundTree( TTree* background, Double_t weight=1.0, Types::ETreeType treetype = Types::kMaxTreeType );
      void AddBackgroundTree( TString datFileB,  Double_t weight=1.0, Types::ETreeType treetype = Types::kMaxTreeType );
      void AddBackgroundTree( TTree* background, Double_t weight, const TString & treetype );

      // ... depreciated, kept for backwards compatibility
      void SetBackgroundTree( TTree* background, Double_t weight=1.0 );

      void SetSignalWeightExpression( const TString& variable );
      void SetBackgroundWeightExpression( const TString& variable );

      // special case: regression
      void AddRegressionTree( TTree* tree, Double_t weight = 1.0,  
                              Types::ETreeType treetype = Types::kMaxTreeType ) { 
         AddTree( tree, "Regression", weight, "", treetype ); 
      }

      // general

      // Data input related
      void SetTree( TTree* tree, const TString& className, Double_t weight ); // depreciated
      void AddTree( TTree* tree, const TString& className, Double_t weight=1.0,
                    const TCut& cut = "",
                    Types::ETreeType tt = Types::kMaxTreeType );
      void AddTree( TTree* tree, const TString& className, Double_t weight, const TCut& cut, const TString& treeType );

      // set input variable
      void SetInputVariables  ( std::vector<TString>* theVariables ); // depreciated
      void AddVariable        ( const TString& expression, const TString& title, const TString& unit,
                                char type='F', Double_t min = 0, Double_t max = 0 );
      void AddVariable        ( const TString& expression, char type='F',
                                Double_t min = 0, Double_t max = 0 );
      void AddTarget          ( const TString& expression, const TString& title = "", const TString& unit = "",
                                Double_t min = 0, Double_t max = 0 );
      void AddRegressionTarget( const TString& expression, const TString& title = "", const TString& unit = "",
                                Double_t min = 0, Double_t max = 0 )
      {
         AddTarget( expression, title, unit, min, max );
      }
      void AddSpectator         ( const TString& expression, const TString& title = "", const TString& unit = "",
                                  Double_t min = 0, Double_t max = 0 );

      // set weight for class
      void SetWeightExpression( const TString& variable, const TString& className = "" );

      // set cut for class
      void SetCut( const TString& cut, const TString& className = "" );
      void SetCut( const TCut& cut, const TString& className = "" );
      void AddCut( const TString& cut, const TString& className = "" );
      void AddCut( const TCut& cut, const TString& className = "" );


      //  prepare input tree for training
      void PrepareTrainingAndTestTree( const TCut& cut, const TString& splitOpt );
      void PrepareTrainingAndTestTree( TCut sigcut, TCut bkgcut, const TString& splitOpt );

      // ... deprecated, kept for backwards compatibility 
      void PrepareTrainingAndTestTree( const TCut& cut, Int_t Ntrain, Int_t Ntest = -1 );

      void PrepareTrainingAndTestTree( const TCut& cut, Int_t NsigTrain, Int_t NbkgTrain, Int_t NsigTest, Int_t NbkgTest, 
                                       const TString& otherOpt="SplitMode=Random:!V" );

      void PrepareTrainingAndTestTree( int foldNumber, Types::ETreeType tt );

      void PrepareFoldDataSet( UInt_t foldNumber, Types::ETreeType tt);
      void MakeKFoldDataSet(UInt_t numberFolds, bool validationSet=false);
      std::vector<std::vector<TMVA::Event*>> SplitSets(std::vector<TMVA::Event*>& oldSet, int seedNum, int numFolds);

      const DataSetInfo& GetDefaultDataSetInfo(){ return DefaultDataSetInfo(); }
 
      //Copy method use in VI and CV DEPRECATED: you can just call Clone  DataLoader *dl2=(DataLoader *)dl1->Clone("dl2")
      DataLoader* MakeCopy(TString name);
      friend void DataLoaderCopy(TMVA::DataLoader* des, TMVA::DataLoader* src);      
 
   private:

 
      DataInputHandler&        DataInput() { return *fDataInputHandler; }
      DataSetInfo&             DefaultDataSetInfo();
      void                     SetInputTreesFromEventAssignTrees();


   private:

      // data members


      DataSetManager* fDataSetManager; // DSMTEST

 
      DataInputHandler*                         fDataInputHandler;//->

      std::vector<TMVA::VariableTransformBase*> fDefaultTrfs;     // list of transformations on default DataSet

      // cd to local directory
      TString                                   fOptions;         // option string given by construction (presently only "V")
      TString                                   fTransformations; // List of transformations to test
      Bool_t                                    fVerbose;         // verbose mode

      // flag determining the way training and test data are assigned to DataLoader
      enum DataAssignType { kUndefined = 0, 
                            kAssignTrees,
                            kAssignEvents };
      DataAssignType                            fDataAssignType;  // flags for data assigning
      std::vector<TTree*>                       fTrainAssignTree; // for each class: tmp tree if user wants to assign the events directly
      std::vector<TTree*>                       fTestAssignTree;  // for each class: tmp tree if user wants to assign the events directly

      std::vector<std::vector<TMVA::Event*>>          fTrainSigEvents;
      std::vector<std::vector<TMVA::Event*>>          fTrainBkgEvents;
      std::vector<std::vector<TMVA::Event*>>          fValidSigEvents;
      std::vector<std::vector<TMVA::Event*>>          fValidBkgEvents;
      std::vector<std::vector<TMVA::Event*>>          fTestSigEvents;
      std::vector<std::vector<TMVA::Event*>>          fTestBkgEvents;

      Int_t                                     fATreeType;          // type of event (=classIndex)
      Float_t                                   fATreeWeight;        // weight of the event
      std::vector<Float_t>                      fATreeEvent;         // event variables
      
      Types::EAnalysisType                      fAnalysisType;    // the training type

      Bool_t                                    fMakeFoldDataSet;

   protected:

      ClassDef(DataLoader,2);
   };
   //utility function to copy dataloaders DEPRECATED: you can just call Clone  DataLoader *dl2=(DataLoader *)dl1->Clone("dl2")
   void DataLoaderCopy(TMVA::DataLoader* des, TMVA::DataLoader* src);
} // namespace TMVA

#endif

