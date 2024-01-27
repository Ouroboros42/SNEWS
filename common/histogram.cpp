#include "common.hpp"

#include <iostream>
#include <cmath>

using namespace std;

Histogram::Histogram(float * data, int ndat, int inNbins, float inLo, float inHi) {
  //data = new float [sizeof(indata)];
  nbins= inNbins;
  lo = inLo;
  hi = inHi;

  hist = new float[nbins];
  // initialize histogram
  for(int j=0; j<nbins; j++) {
    hist[j]=0.0;
  }
  // do counting and create bins
  float delta = (hi-lo)/nbins;
  datsize = ndat;

  // fill histogram ..
  for(int k=0; k<datsize; k++) {
    int bin_n = (data[k]-lo)/delta;
    hist[bin_n] +=1;
  }
}

  // // print number of entries in ith bin
  //   void printBinContents(int i){
  //     std::cout <<  " Bin = " << i << " n_i = " <<  this->hist[i]<< std::endl;
  //   }
    
  //   // fill in all the bins
  //   void printAllBins() {
  //     for(int i=0; i<nbins; i++) {
	//       std::cout << " Entries in bin i = "<< i << " n_i = " << this->hist[i] << std::endl; 
  //     }
  //   }
    
  //   // print upper edge
  //   void upperEdge(){
  //     std::cout << " Upper edge " << this->hi << std::endl;
  //   }
    
  //   // print lower edge
  //   void lowerEdge(){
  //     std::cout << " Lower edge " << this->lo << std::endl;
  //   }

