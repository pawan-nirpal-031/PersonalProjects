  vector<vector<vector<pair<float, float>>>> computeGradientPyramid(
      const vector<vector<vector<pair<int, int>>>> &scaleSpaceExtremas,
      const vector<vector<vector<vector<float>>>> &scaleSpacePyramid) {
    vector<vector<vector<pair<float, float>>>> gradPyramid;
    for (int oct = 0; oct < scaleSpaceExtremas.size(); oct++) {
      vector<vector<pair<float,float>>> gradOctave;
      for(int scl = 0;scl<scaleSpaceExtremas[oct].size();scl++){
        
      }
    }
  }

  vector<vector<vector<pair<int, int>>>> performExtramaRefinement(
      vector<vector<vector<pair<int, int>>>> &ScaleSpaceExtrema,
      vector<vector<vector<vector<float>>>> &DoGPyramid) {
    vector<vector<vector<pair<float, float>>>> gradientPyramid =
        computeGradientPyramid(ScaleSpaceExtrema, DoGPyramid);
  }