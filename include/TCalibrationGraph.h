#ifndef TCALIBRATIONGRAPH_H
#define TCALIBRATIONGRAPH_H

#include <vector>
#include <string>
#include <iostream>

#include "TNamed.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TList.h"
#include "TLegend.h"

class TCalibrationGraphSet;

class TCalibrationGraph : public TGraphErrors {
public:
	TCalibrationGraph() {}
	TCalibrationGraph(TCalibrationGraphSet* parent, const int& size, const bool& isResidual = false) : TGraphErrors(size), fParent(parent), fIsResidual(isResidual) {}
	TCalibrationGraph(TCalibrationGraphSet* parent, TGraphErrors* graph) : TGraphErrors(*graph), fParent(parent), fIsResidual(false) {}
	~TCalibrationGraph() {}

	using TGraph::RemovePoint; // to use the function with integer index as argument
	Int_t RemovePoint() override; // *MENU*

	void IsResidual(bool val) { fIsResidual = val; }
	bool IsResidual() { return fIsResidual; }

#if ROOT_VERSION_CODE < ROOT_VERSION(6,26,0)
	void Scale(const double& scale);
#endif

	void VerboseLevel(int val) { fVerboseLevel = val; }

private:
	TCalibrationGraphSet* fParent{nullptr}; ///< pointer to the set this graph belongs to
	bool fIsResidual{false}; ///< flag to indicate that this graph is for residuals

	int fVerboseLevel{0}; ///< Changes verbosity from 0 (quiet) to 4 (very verbose)

	ClassDefOverride(TCalibrationGraph, 1)
};

class TCalibrationGraphSet : public TNamed {
public:
	TCalibrationGraphSet(TGraphErrors* graph = nullptr, const std::string& label = "");
	~TCalibrationGraphSet();

	bool SetResidual(const bool& force = false);
	int Add(TGraphErrors*, const std::string& label);

	void SetLineColor(int index, int color)   { fGraphs[index].SetLineColor(color);   fResidualGraphs[index].SetLineColor(color); }   ///< Set the line color of the graph and residuals at index
	void SetMarkerColor(int index, int color) { fGraphs[index].SetMarkerColor(color); fResidualGraphs[index].SetMarkerColor(color); } ///< Set the marker color of the graph and residuals at index

	int GetN() { return fTotalGraph->GetN(); }     ///< Returns GetN(), i.e. number of points of the total graph.
	double* GetX() { return fTotalGraph->GetX(); } ///< Returns an array of x-values of the total graph.
	double* GetY() { return fTotalGraph->GetY(); } ///< Returns an array of y-values of the total graph.
	double* GetEX() { return fTotalGraph->GetEX(); } ///< Returns an array of x-errors of the total graph.
	double* GetEY() { return fTotalGraph->GetEY(); } ///< Returns an array of y-errors of the total graph.

	double GetMinimumX() { return fMinimumX; } ///< Return minimum x-value.
	double GetMaximumX() { return fMaximumX; } ///< Return maximum x-value.
	double GetMinimumY() { return fMinimumY; } ///< Return minimum y-value.
	double GetMaximumY() { return fMaximumY; } ///< Return maximum y-value.

	void Fit(TF1* function, Option_t* opt = "") { fTotalGraph->Fit(function, opt); } ///< Fits the provided function to the total graph.
	TF1* FitFunction() { return reinterpret_cast<TF1*>(fTotalGraph->GetListOfFunctions()->FindObject("fitfunction")); } ///< Gets the calibration from the total graph (might be nullptr!).
	TGraphErrors* TotalGraph() { return fTotalGraph; }
	size_t NumberOfGraphs() { return fGraphs.size(); }
	TCalibrationGraph* Graph(size_t i) { return &(fGraphs.at(i)); }
	TCalibrationGraph* Residual(size_t i) { return &(fResidualGraphs.at(i)); }

	void DrawCalibration(Option_t* opt = "", TLegend* legend = nullptr);
	void DrawResidual(Option_t* opt = "", TLegend* legend = nullptr);

	Int_t RemovePoint();
	Int_t RemoveResidualPoint();

	void Scale(); ///< scale all graphs to fit each other (based on the first graph)

	using TObject::Print;
	void Print(Option_t* opt);

	void ResetTotalGraph(); ///< reset the total graph and add the individual ones again (used e.g. after scaling of individual graphs is done)

	TCalibrationGraphSet& operator=(const TCalibrationGraphSet& rhs)
	{
		/// Assignment operator that takes care of properly cloning all the pointers to objects.
		fGraphs.resize(rhs.fGraphs.size());
		for(size_t i = 0; i < fGraphs.size(); ++i) {
			fGraphs[i] = rhs.fGraphs[i];
		}
		fResidualGraphs.resize(rhs.fResidualGraphs.size());
		for(size_t i = 0; i < fResidualGraphs.size(); ++i) {
			fResidualGraphs[i] = rhs.fResidualGraphs[i];
		}
		fLabel = rhs.fLabel;
		fTotalGraph = static_cast<TGraphErrors*>(rhs.fTotalGraph->Clone());
		fTotalResidualGraph = static_cast<TGraphErrors*>(rhs.fTotalResidualGraph->Clone());
		fGraphIndex = rhs.fGraphIndex;
		fPointIndex = rhs.fPointIndex;
		fResidualSet = rhs.fResidualSet;
		fName = rhs.fName;
		return *this;
	}

	void VerboseLevel(int val) { fVerboseLevel = val; for(auto graph : fGraphs) graph.VerboseLevel(val); for(auto graph : fResidualGraphs) graph.VerboseLevel(val); }

private:
	std::vector<TCalibrationGraph> fGraphs; ///< These are the graphs used for plotting the calibration points per source.
	std::vector<TCalibrationGraph> fResidualGraphs; ///< These are the graphs used for plotting the residuals per source.
	std::vector<std::string> fLabel; ///< The labels for the different graphs.
	TGraphErrors* fTotalGraph{nullptr}; ///< The sum of the other graphs, used for fitting.
	TGraphErrors* fTotalResidualGraph{nullptr}; ///< The sum of the residuals. Not really used apart from plotting (but overlayed with the individual graphs).
	std::vector<size_t> fGraphIndex; ///< Index of the graph this point belongs to.
	std::vector<size_t> fPointIndex; ///< Index of the point within the graph this point corresponds to.
	bool fResidualSet{false}; ///< Flag to indicate if the residual has been set correctly.
	double fMinimumX{0.}; ///< Minimum x-value
	double fMaximumX{0.}; ///< Maximum x-value
	double fMinimumY{0.}; ///< Minimum y-value
	double fMaximumY{0.}; ///< Maximum y-value

	int fVerboseLevel{0}; ///< Changes verbosity from 0 (quiet) to 4 (very verbose)

	ClassDefOverride(TCalibrationGraphSet, 3)
};
#endif
