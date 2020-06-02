// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS/"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Dorrestein Lab - University of California San Diego - https://dorresteinlab.ucsd.edu/$
// $Authors: Abinesh Sarvepalli and Louis Felix Nothias$
// $Contributors: Fabian Aicheler and Oliver Alka from Oliver Kohlbacher's group at Tubingen University$
// --------------------------------------------------------------------------

//----------------------------------------------------------
// Doxygen docu
//----------------------------------------------------------
/**
  @page TOPP_GNPSExport GNPSExport

  @brief Export MS/MS data in .MGF format for GNPS (http://gnps.ucsd.edu).

GNPS (Global Natural Products Social Molecular Networking, http://gnps.ucsd.edu) is an open-access knowledge base for community-wide organisation and sharing of raw, processed or identified tandem mass (MS/MS) spectrometry data. The GNPS web-platform makes possible to perform spectral library search against public MS/MS spectral libraries, as well as to perform various data analysis such as MS/MS molecular networking, network annotation propagation (http://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1006089), and the Dereplicator-based annotation (https://www.nature.com/articles/nchembio.2219). The GNPS manuscript is available here: https://www.nature.com/articles/nbt.3597

This tool was developed for the Feature Based Molecular Networking (FBMN) workflow on GNPS (https://gnps.ucsd.edu/ProteoSAFe/static/gnps-splash2.jsp)

Please cite our preprint: Nothias, L.F. et al, Feature-based Molecular Networking in the GNPS Analysis Environment
bioRxiv 812404 (2019) (https://www.biorxiv.org/content/10.1101/812404v1)

See the FBMN workflow documentation here (https://ccms-ucsd.github.io/GNPSDocumentation/featurebasedmolecularnetworking/)

In brief, after running an OpenMS "metabolomics" pipeline, the GNPSExport TOPP tool can be used
on the consensusXML file and corresponding mzML files to generate the files needed for FBMN on GNPS.
These two files are:

	- The MS/MS spectral data file (.MGF format) which is generated  with the GNPSExport util.
	- The feature quantification table (.TXT format) which is generated with the TextExport util.

For each consensusElement in the consensusXML file, the GNPSExport produces one representative consensus
MS/MS spectrum (named peptide annotation in OpenMS jargon) outputed in the MS/MS spectral file (.MGF file).
Several modes for the generation of the consensus MS/MS spectrum are available and described below.
Note that these parameters are defined in the GNPSExport INI parameters file.

Representative command:
@code
GNPSExport -ini iniFile-GNPSExport.ini -in_cm filefilter.consensusXML -in_mzml inputFile0.mzML inputFile1.mzML -out GNPSExport_output.mgf
@endcode

The GNPSExport TOPP tool can be run on a consensusXML file and the corresponding mzML files to generate a MS/MS spectral file (MGF format)
and corresponding feature quantification table (.TXT format) that contains the LC-MS peak area intensity.

Requirements:
	- The IDMapper has to be ran on the featureXML files, in order to associate MS2 scan(s) (peptide annotation) with each
	features. These peptide annotations are used by the GNPSExport.
	- The FileFilter has to be ran on the consensusXML file, prior to the GNPSExport, in order to remove consensusElements
	without MS2 scans (peptide annotation).

Parameters:
	- Binning (ms2_bin_size): Defines the binning width of fragment ions during the merging of eligible MS/MS spectra.
	- Cosine Score Treshold (merged_spectra:cos_similarity): Defines the necessary pairwise cosine similarity with the highest precursor intensity MS/MS scan.

  - Output Type (output_type):
Options for outputing GNPSExport spectral processing are:
    -# [RECOMMENDED] merged_spectra
      For each consensusElement, the GNPSExport will merge all the eligible MS/MS scans into one representative consensus MS/MS spectrum.
      Eligible MS/MS scans have a pairwise cosine similarity with the MS/MS scan of highest precursor intensity above the Cosine Similarity Treshold.
	    The fragment ions of merged MS/MS scans are binned in m/z (or Da) range defined by the Binning width parameter.    
      .
	  -# Most intense: most_intense - For each consensusElement, the GNPSExport will output the most intense MS/MS scan (with the highest precursor ion intensity) as consensus MS/MS spectrum.
      .    

Note that mass accuracy and the retention time window for the pairing between MS/MS scans and a LC-MS feature
or consensusElement is defined at the IDMapper tool step.

A representative OpenMS-GNPS workflow would sequencially use these OpenMS TOPP tools:
  1. Input mzML files
  2. Run the FeatureFinderMetabo tool on the mzML files.
  3. Run the IDMapper tool on the featureXML and mzML files.
  4. Run the MapAlignerPoseClustering tool on the featureXML files.
  5. Run the MetaboliteAdductDecharger on the featureXML files.
  6. Run the FeatureLinkerUnlabeledKD tool or FeatureLinkerUnlabeledQT, on the featureXML files and output a consensusXML file.
  8. Run the FileFilter on the consensusXML file to keep only consensusElements with at least MS/MS scan (peptide identification).
  9. Run the GNPSExport on the "filtered consensusXML file" to export an .MGF file.
  10. Run the TextExport on the "filtered consensusXML file" to export an .TXT file.
  11. Upload your files to GNPS and run the Feature-Based Molecular Networking workflow. Instructions are here:
https://ccms-ucsd.github.io/GNPSDocumentation/featurebasedmolecularnetworking/

The GitHub for that ProteoSAFe workflow and an OpenMS python wrappers is available here:
https://github.com/Bioinformatic-squad-DorresteinLab/openms-gnps-workflow

An online version of the OpenMS-GNPS pipeline for FBMN running on CCMS server (http://proteomics.ucsd.edu/) is available on GNPS:
https://ccms-ucsd.github.io/GNPSDocumentation/featurebasedmolecularnetworking-with-OpenMS

GNPS (Global Natural Products Social Molecular Networking, https://gnps.ucsd.edu/ProteoSAFe/static/gnps-splash2.jsp)
is an open-access knowledge base for community-wide organisation and sharing of raw, processed
or identified tandem mass (MS/MS) spectrometry data.
The GNPS web-platform makes possible to perform spectral library search against public MS/MS spectral libraries,
as well as to perform various data analysis such as MS/MS molecular networking, Network Annotation Propagation
Network Annotation Propagation (http://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1006089)
and the DEREPLICATOR (https://www.nature.com/articles/nchembio.2219)
The GNPS paper is available here (https://www.nature.com/articles/nbt.3597)

  <B>The command line parameters of this tool are:</B>
  @verbinclude TOPP_GNPSExport.cli
  <B>INI file documentation of this tool:</B>
  @htmlinclude TOPP_GNPSExport.html
 */

#include <OpenMS/ANALYSIS/ID/PrecursorPurity.h>
#include <OpenMS/APPLICATIONS/TOPPBase.h>
#include <OpenMS/COMPARISON/SPECTRA/BinnedSpectrum.h>
#include <OpenMS/COMPARISON/SPECTRA/BinnedSpectralContrastAngle.h>
#include <OpenMS/CONCEPT/UniqueIdInterface.h>
#include <OpenMS/FILTERING/TRANSFORMERS/SpectraMerger.h>
#include <OpenMS/FORMAT/ConsensusXMLFile.h>
#include <OpenMS/FORMAT/MzMLFile.h>
#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/KERNEL/MSSpectrum.h>
#include <iostream>
#include <fstream>

using namespace OpenMS;
using namespace std;

class TOPPGNPSExport : public TOPPBase
{
public:
  TOPPGNPSExport() : TOPPBase(
    "GNPSExport", 
    "Tool to export representative consensus MS/MS scan per consensusElement into a .MGF file format.\nSee the documentation on https://ccms-ucsd.github.io/GNPSDocumentation/featurebasedmolecularnetworking_with_openms", 
    true,
    {
      { 
        "Nothias L.F. et al.", // authors
        "Feature-based Molecular Networking in the GNPS Analysis Environment", // title
        "bioRxiv 812404 (2019)", // when_where
        "10.1101/812404" // doi
      }
    }
  ) {}


private:
  static constexpr double DEF_COSINE_SIMILARITY = 0.9;
  static constexpr double DEF_MERGE_BIN_SIZE = static_cast<double>(BinnedSpectrum::DEFAULT_BIN_WIDTH_HIRES);

  static constexpr double DEF_PREC_MASS_TOL = 0.5;
  static constexpr bool DEF_PREC_MASS_TOL_ISPPM = false;

  static constexpr double DEF_PEPT_CUTOFF = 5;
  static constexpr double DEF_MSMAP_CACHE = 50;


  void writeMS2BlockToFile(
    ofstream& output_file,
    const map<double,int> ms2_block, 
    const String& output_type,
    const int scan_index,
    const String feature_id,
    const String feature_charge,
    const String feature_mz,
    const String spec_index,
    const String feature_rt
  ) 
  {
    output_file << "BEGIN IONS" << "\n";
    output_file << "OUTPUT=" << output_type << "\n";
    
    output_file << "SCANS=" << scan_index << "\n";
    output_file << "FEATURE_ID=" << feature_id << "\n";

    output_file << "MSLEVEL=2" << "\n";
    output_file << "CHARGE=" << feature_charge << "\n";
    output_file << "PEPMASS=" << feature_mz << "\n";
    output_file << "FILE_INDEX=" << spec_index << "\n";
    output_file << "RTINSECONDS=" << feature_rt << "\n";

    for (const pair<double,int>& ms2 : ms2_block)
    {
      if (ms2.second > 0)
      {
        output_file << ms2.first << "\t" << ms2.second << "\n";
      }
    }

    output_file << "END IONS" << "\n\n";
  }

  void generateMSMSSpectrumBins(
    const vector<pair<double,int>>& sorted_mz_int_pairs,
    double delta_mz,
    map<double,int>& ms2_block
  )
  {
    // generate new spectrum
    vector<double> mz_merged;
    vector<double> intensity_merged;

    // double last_mz = numeric_limits<double>::min();
    double last_mz = sorted_mz_int_pairs[0].first;    
    double sum_mz = 0;
    int sum_intensity = 0;
    double count = 0;
    for (const auto mz_int : sorted_mz_int_pairs)
    {
      if (abs(mz_int.first - last_mz) > delta_mz && count > 0)
      {
        if (sum_intensity > 0)
        {
          double mz_merged = sum_mz/count;
          int int_merged = sum_intensity;
          ms2_block[mz_merged] = int_merged;
        }

        last_mz = mz_int.first;
        sum_mz = 0;
        count = 0;
        sum_intensity = 0;
      }

      sum_mz += mz_int.first;
      sum_intensity += mz_int.second;
      count++;
    }
    //remaining scans in last bucket
    if (count > 0 && sum_intensity > 0)
    {
      double mz_merged = sum_mz/count;
      double int_merged = sum_intensity;

      ms2_block[mz_merged] = int_merged;
    }

    // return would be the reformatted map<double,int> ms2_block passed in by value
  }

  void sortElementMapsByIntensity(const ConsensusFeature& feature, vector<pair<int,double>>& element_maps)
  {    
    // convert element maps to vector of pair<int,double>(map, intensity)     
    for (ConsensusFeature::HandleSetType::const_iterator feature_iter = feature.begin();\
          feature_iter != feature.end(); ++feature_iter)
    {
      element_maps.emplace_back(feature_iter->getMapIndex(), feature_iter->getIntensity());
    }

    // sort elements by intensity
    sort(element_maps.begin(), element_maps.end(), [](const pair<int,double> &a, const pair<int,double> &b)
    {
      return a.second > b.second;
    });

    // return value will be reformatted vector<int> element_maps passed in by value
  }

  void getElementPeptideIdentificationsByElementIntensity(
    const ConsensusFeature& feature,
    const vector<pair<int,double>>& sorted_element_maps,
    vector<pair<int,int>>& pept_indices
  )
  {
    for (const pair<int,double> element_pair : sorted_element_maps)
    {
      int element_map = element_pair.first;

      vector<PeptideIdentification> feature_pepts = feature.getPeptideIdentifications();
      for (const PeptideIdentification pept_id : feature_pepts)
      {
        if (pept_id.metaValueExists("spectrum_index") && pept_id.metaValueExists("map_index")
            && (int)pept_id.getMetaValue("map_index") == element_map)
        {
          int map_index = pept_id.getMetaValue("map_index");
          int spec_index = pept_id.getMetaValue("spectrum_index");
          pept_indices.emplace_back(map_index,spec_index);
          break;
        }
      }
    }

    // return will be reformatted vector<PeptideIdentification> pepts passed in by value
  }

  MSExperiment& getSpectraAtIndex(const StringList& mzml_file_paths, vector<MSExperiment>& specs_list, int map_index) {
    MSExperiment& specs = specs_list.at(map_index);
    
    if (specs.empty())
    {
      MzMLFile mzml_file;
      mzml_file.load(mzml_file_paths[map_index], specs);
    }

    return specs_list.at(map_index);
  }


protected:
  // this function will be used to register the tool parameters
  // it gets automatically called on tool execution
  void registerOptionsAndFlags_() override
  {
    registerInputFile_("in_cm", "<file>", "", "Input consensusXML file containing only consensusElements with \"peptide\" annotations.");
    setValidFormats_("in_cm", ListUtils::create<String>("consensusXML"));

    registerInputFileList_("in_mzml", "<files>", ListUtils::create<String>(""), "Original mzml files containing the ms2 spectra (aka peptide annotation). \nMust be in order that the consensusXML file maps the original mzML files.");
    setValidFormats_("in_mzml", ListUtils::create<String>("mzML"));

    registerOutputFile_("out", "<file>", "", "Output MGF file");
    setValidFormats_("out", ListUtils::create<String>("mgf"));

    registerStringOption_("output_type", "<choice>", "most_intense", "specificity of mgf output information", false);
    setValidStrings_("output_type", ListUtils::create<String>("merged_spectra,most_intense"));

    addEmptyLine_();

    // registerIntOption_("msmap_cache", "<num>", DEF_MSMAP_CACHE, "Number of msmaps that can be cached during export for optimized performance", false, true);
    registerIntOption_("peptide_cutoff", "<num>", DEF_PEPT_CUTOFF, "Number of most intense peptides to consider per consensus element; '-1' to consider all identifications", false, true);
    registerDoubleOption_("ms2_bin_size", "<num>", DEF_MERGE_BIN_SIZE, "Bin size (Da) for fragment ions when merging ms2 scans", false, false);

    // addEmptyLine_();

    registerTOPPSubsection_("merged_spectra", "Options for exporting mgf file with merged spectra per consensusElement");
    registerDoubleOption_("merged_spectra:precursor_mass_tolerance", "<num>", DEF_PREC_MASS_TOL, "Precursor mass tolerance (Da) for ms annotations", false);
    registerDoubleOption_("merged_spectra:cos_similarity", "<num>", DEF_COSINE_SIMILARITY, "Cosine similarity threshold for merged_spectra output", false);
  }

  // the main function is called after all parameters are read
  ExitCodes main_(int, const char**) override
  {
    //-------------------------------------------------------------
    // parsing parameters
    //-------------------------------------------------------------
    // int max_msmap_cache(getIntOption_("msmap_cache"));
    int pept_cutoff(getIntOption_("peptide_cutoff"));

    double cos_sim_threshold(getDoubleOption_("merged_spectra:cos_similarity"));
    double bin_width(getDoubleOption_("ms2_bin_size"));

    String consensus_file_path(getStringOption_("in_cm"));
    StringList mzml_file_paths = getStringList_("in_mzml");
    String out(getStringOption_("out"));
    String output_type(getStringOption_("output_type"));
    
    ofstream output_file(out);

    ProgressLogger progress_logger;
    progress_logger.setLogType(log_type_);


    //-------------------------------------------------------------
    // reading input
    //-------------------------------------------------------------
    // ConsensusMap
    ConsensusXMLFile consensus_file;
    ConsensusMap consensus_map;
    consensus_file.load(consensus_file_path, consensus_map);


    //-------------------------------------------------------------
    // preprocessing: allocate memory
    //-------------------------------------------------------------
    // max_msmap_cache = std::min(max_msmap_cache, static_cast<int>(mzml_file_paths.size()));
    int max_msmap_cache = static_cast<int>(mzml_file_paths.size());
    vector<MSExperiment> specs_list(max_msmap_cache);


    //-------------------------------------------------------------
    // write output (+ merge computations)
    //-------------------------------------------------------------    
    progress_logger.startProgress(0, consensus_map.size(), "parsing features and ms2 identifications...");
    for (Size cons_i = 0; cons_i < consensus_map.size(); ++cons_i)    
    {
      const ConsensusFeature feature = consensus_map[cons_i];

      //
      // determine feature's charge
      //
      BaseFeature::ChargeType charge = feature.getCharge();  
      for (ConsensusFeature::HandleSetType::const_iterator feature_iter = feature.begin();\
            feature_iter != feature.end(); ++feature_iter)
      {
        // determine feature's charge
        if (feature_iter->getCharge() > charge) {
          charge = feature_iter->getCharge();
        }
      }

      //
      // compute most intense peptide identifications (based on precursor intensity)
      //
      vector<pair<int,double>> element_maps;
      sortElementMapsByIntensity(feature, element_maps);
      vector<pair<int,int>> pepts;
      getElementPeptideIdentificationsByElementIntensity(feature, element_maps, pepts);

      if (output_type == "most_intense")
      {
        // read most_intense scan info
        int map_index = (pepts[0]).first;
        int spec_index = (pepts[0]).second;      

        // printf("map_index %d\n", map_index);

        // MSExperiment specs = specs_list->at(map_index);
        MSExperiment& specs = getSpectraAtIndex(mzml_file_paths, specs_list, map_index);
        // if (specs.empty())
        // {
        //   mzml_file.load(mzml_file_paths[map_index], specs);
        // }
        auto spec = specs[spec_index];
        spec.sortByIntensity(true);

        vector<pair<double,int>> mz_int_pairs;
        for (auto spec_iter=spec.begin(); spec_iter!=spec.end(); ++spec_iter)        
        {
          mz_int_pairs.emplace_back(spec_iter->getMZ(), static_cast<int>(spec_iter->getIntensity()));
        }
        sort(mz_int_pairs.begin(), mz_int_pairs.end());
        
        map<double,int> ms2_block;
        generateMSMSSpectrumBins(mz_int_pairs, bin_width, ms2_block);      

        // write output
        writeMS2BlockToFile(output_file, ms2_block, output_type, (cons_i+1), feature.getUniqueId(),
                            charge, feature.getMZ(), spec_index, spec.getRT());

        mz_int_pairs.clear();
      }      
      else if (output_type == "merged_spectra")
      {
        // discard poorer precursor spectra for 'merged_spectra' and 'full_spectra' output
        if (pepts.size() > (unsigned long) pept_cutoff) { pepts.erase(pepts.begin()+pept_cutoff, pepts.end()); }                

        int best_mapi = pepts[0].first;
        int best_speci = pepts[0].second;

        MSExperiment& best_specs = getSpectraAtIndex(mzml_file_paths, specs_list, best_mapi);
        auto& best_spec = best_specs[best_speci];
        BinnedSpectrum binned_highest_int(best_spec, BinnedSpectrum::DEFAULT_BIN_WIDTH_HIRES, false, 1, BinnedSpectrum::DEFAULT_BIN_OFFSET_HIRES);

        vector<pair<double,int>> mz_int_pairs;

        for (pair<int,int> pept : pepts)
        {
          int map_index = pept.first;
          int spec_index = pept.second;

          MSExperiment& specs = getSpectraAtIndex(mzml_file_paths, specs_list, map_index);

          auto& test_spec = specs[spec_index];
          const BinnedSpectrum binned_spectrum(test_spec, BinnedSpectrum::DEFAULT_BIN_WIDTH_HIRES, false, 1, BinnedSpectrum::DEFAULT_BIN_OFFSET_HIRES);

          BinnedSpectralContrastAngle bsca;
          double cos_sim = bsca(binned_highest_int, binned_spectrum);

          if (cos_sim >= cos_sim_threshold)
          {
            for (auto spec_iter = test_spec.begin(); spec_iter < test_spec.end(); spec_iter++)
            {
              mz_int_pairs.emplace_back(spec_iter->getMZ(), spec_iter->getIntensity());
            }
          }
        }
        sort(mz_int_pairs.begin(), mz_int_pairs.end());

        map<double,int> ms2_block;
        generateMSMSSpectrumBins(mz_int_pairs, bin_width, ms2_block);

        // write output
        writeMS2BlockToFile(output_file, ms2_block, output_type, (cons_i+1), feature.getUniqueId(),
                            charge, feature.getMZ(), best_speci, feature.getRT());

        // delete allocated resources
        mz_int_pairs.clear();         
      

        element_maps.clear();
        pepts.clear();
      }
      
      progress_logger.setProgress(cons_i);
    } // end of for-loop across features
    progress_logger.endProgress();

    // delete / dealloc mem resources
    output_file.close();

    mzml_file_paths.clear();
    // delete [] specs_list;
    specs_list.clear();
    
    return EXECUTION_OK;
  }
};

// the actual main functioned needed to create an executable
int main (int argc, const char** argv)
{
  TOPPGNPSExport tool;
  return tool.main(argc, argv);
}
/// @endcond
