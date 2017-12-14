// list of taggers (legacy)

const int ntag = 1;
  std::string tags[ntag] = {
    "MV2c10", "MV2c20"
    /* "sv0", "mv1c", "jfitCOMBNN", "mv1", "jfitCOMBNN_c", "jfitLog" */
  };

  const char* tags_ex[ntag] = {
    "MV2c10", "MV2c20"
    /* "SV0", "MV1c", "JetFitterCOMBNN", "MV1", "JetFitterCOMBNNc", "JetFitterLog" */
  };
  const int nw = 8;
  double dw[ntag*nw] = {
    -0.9185, -0.7887, -0.5911, -0.4434, -0.0436, 0.4496, 0.7535, 0.9540, // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/BTaggingBenchmarks#b_tagging_Benchmarks_for_tagger
    /* 0.00,2.75,5.65,7.85,9.5,-999,-999, */
    // 0.0836,0.4051,0.7068,0.8349,0.8641,0.9195,0.9853,
    // -2.55, -0.95, 0.55,1.0,-999,-999,-999,
    // 0.0617, 0.3511, 0.7892, 0.9827, 0.993981, 0.994450, -999, //2012-2013 winter
    // 0.8641,0.9195, -999, -999,-999,-999,-999,
    // 0.5,1.5,2.5,-999,-999,-999,-999
  };


  int docalib[ntag*nw] = {
    0, 0, 1, 0, 0, 0, 0,
    // 1, 1, 1, 1, 1, 1, 1,
    // 1, 1, 1, 1, 0, 0, 0,
    // 1, 1, 1, 1, 1, 1, 0,
    // 1, 1, 0, 0, 0, 0, 0,
    // 1, 1, 1, 0, 0, 0, 0
  };

  std::string s_operation[ntag*nw] = {
    "OP70", "OP60", "OP50", "OP40", "OP30", "OP00", "OP00",
    // "OP90", "OP80", "OP70", "OP60", "OP57", "OP50", "OP30",
    // "OP80", "OP70", "OP60", "OP57", "OP00", "OP00", "OP00",
    // "OP90", "OP80", "OP70", "OP60", "OP50", "OP30", "OP00",
    // "OP55", "OP50", "OP00", "OP00", "OP00", "OP00", "OP00",
    // "OP61", "OP51", "OP41", "OP00", "OP00", "OP00", "OP00"
  };

  const char* method = "negtag";
  const int nt = ntag;
