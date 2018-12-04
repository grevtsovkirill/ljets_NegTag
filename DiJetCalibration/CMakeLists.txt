#
# Build configuration for the DiJetCalibration package
#

# Set the name of the package:
atlas_subdir( DiJetCalibration )

# Set up which packages this package depends on:
atlas_depends_on_subdirs(
   PUBLIC
   PhysicsAnalysis/TopPhys/xAOD/TopAnalysis
   PRIVATE
   PhysicsAnalysis/TopPhys/xAOD/TopEvent
   PhysicsAnalysis/TopPhys/xAOD/TopEventSelectionTools
   PhysicsAnalysis/TopPhys/xAOD/TopConfiguration
   Event/xAOD/xAODBTagging
)

# External(s) used by the package:
find_package( ROOT COMPONENTS Core Hist Physics REQUIRED )

# Generate a dictionary for the library:
atlas_add_root_dictionary( DiJetCalibrationLib DiJetCalibrationLibDictSrc
   ROOT_HEADERS    DiJetCalibration/DiJetEventSaver.h Root/LinkDef.h
   EXTERNAL_PACKAGES ROOT )

# Build the shared library of the package:
atlas_add_library( DiJetCalibrationLib
   DiJetCalibration/*.h Root/*.h Root/*.cxx ${DiJetCalibrationLibDictSrc}
   PUBLIC_HEADERS DiJetCalibration
   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
   LINK_LIBRARIES TopAnalysis TopEvent TopEventSelectionTools TopConfiguration xAODBTaggingEfficiencyLib
   PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES}
)

# Install the script(s) of the package:
atlas_install_scripts( scripts/* macros/*)

# Install the Python module(s) of the package:
atlas_install_python_modules( python/*.py )

# Install data files from the package:
atlas_install_data( data/* )
