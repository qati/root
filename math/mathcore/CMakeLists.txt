############################################################################
# CMakeLists.txt file for building ROOT io/io package
############################################################################

set(MATHCORE_HEADERS TRandom.h
  TRandom1.h TRandom2.h TRandom3.h TKDTree.h TKDTreeBinning.h TStatistic.h
  Math/IParamFunction.h Math/IFunction.h Math/ParamFunctor.h Math/Functor.h
  Math/Minimizer.h Math/MinimizerOptions.h Math/IntegratorOptions.h Math/IOptions.h Math/GenAlgoOptions.h
  Math/BasicMinimizer.h Math/MinimTransformFunction.h Math/MinimTransformVariable.h
  Math/Integrator.h Math/VirtualIntegrator.h Math/AllIntegrationTypes.h Math/AdaptiveIntegratorMultiDim.h
  Math/IntegratorMultiDim.h Math/Factory.h Math/FitMethodFunction.h Math/GaussIntegrator.h
  Math/GaussLegendreIntegrator.h Math/RootFinder.h Math/IRootFinderMethod.h Math/RichardsonDerivator.h
  Math/BrentMethods.h Math/BrentMinimizer1D.h Math/BrentRootFinder.h Math/DistSampler.h
  Math/DistSamplerOptions.h Math/GoFTest.h Math/SpecFuncMathCore.h Math/DistFuncMathCore.h
  Math/ChebyshevPol.h Math/KDTree.h Math/TDataPoint.h Math/TDataPointN.h Math/Delaunay2D.h
  Math/Random.h Math/TRandomEngine.h Math/RandomFunctions.h Math/StdRandomEngines.h
  Math/MersenneTwisterEngine.h Math/MixMaxEngine.h 
)

ROOT_GENERATE_DICTIONARY(G__MathCore   TComplex.h TMath.h ${MATHCORE_HEADERS} Fit/*.h MODULE MathCore LINKDEF LinkDef.h OPTIONS "-writeEmptyRootPCM")

add_definitions(-DUSE_ROOT_ERROR )
ROOT_ADD_C_FLAG(_flags -Wno-strict-overflow)  # Avoid what it seems a compiler false positive warning
ROOT_ADD_C_FLAG(_flags -Wno-maybe-uninitialized)  # Avoid what it seems a compiler false positive warning
ROOT_ADD_C_FLAG(_flags -Wno-parentheses-equality)

set_source_files_properties(src/triangle.c COMPILE_FLAGS "${_flags}")

ROOT_LINKER_LIBRARY(MathCore *.cxx *.c G__MathCore.cxx LIBRARIES ${CMAKE_THREAD_LIBS_INIT} DEPENDENCIES Core)

ROOT_INSTALL_HEADERS()

ROOT_ADD_TEST_SUBDIRECTORY(test)




