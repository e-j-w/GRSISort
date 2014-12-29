#ifndef __TGRSIFIT_H
#define __TGRSIFIT_H

#include "TGRSIFunctions.h"
#include "TObject.h"
#include "TH1.h"
#include "TF1.h"
#include "TList.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVirtualFitter.h"
#include "TMath.h"
#include "TNamed.h"
#include "TROOT.h"
#include <utility>

using namespace TGRSIFunctions;

class TGRSIFit : public TF1 {
 public:
   virtual ~TGRSIFit(){};

 protected: 
   TGRSIFit();
   //TGRSIFit(const char *name,Double_t (*fcn)(Double_t *, Double_t *), Double_t xmin, Double_t xmax, Int_t npar) : TF1(name, fcn, xmin, xmax, npar){};
   TGRSIFit(TF1 fitfunc) : TF1(fitfunc){};
   TGRSIFit(const TGRSIFit &copy);
  
 public:
   //Every fit object should have to initialize parameters and have a fit method defined.
 //  virtual Double_t Fit(Option_t *opt = "") = 0;
   virtual Bool_t InitParams(TH1*) = 0;
   Bool_t IsGoodFit() const { return goodfit_flag; }
   virtual void SetHistogram(TH1* hist){fHistogram = hist;} //fHistogram is a member of TF1. I'm not sure this does anything proper right now

 protected:
   Bool_t IsInitialized() const { return init_flag; }
   void SetInitialized(Bool_t flag = true) {init_flag = flag;}
   void GoodFit(Bool_t flag = true) { goodfit_flag = flag; }

 private:
   Bool_t init_flag;
   Bool_t goodfit_flag; //This doesn't do anything yet

 public:  
   virtual void Print(Option_t *opt = "") const;
   virtual void Clear();

   ClassDef(TGRSIFit,0);
};

#endif
