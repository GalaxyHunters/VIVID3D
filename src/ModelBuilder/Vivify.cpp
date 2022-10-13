#include "Vivify.h"

namespace vivid{
    CModel vivifyMesh(const std::vector<CPoint> &arInputPoints, const std::vector<bool> &arMask, const std::string &arOutputFilePath,
                   std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                   normal_float aOpacity, coord_t aNoiseDisplacement) {
        CVoronoiVolume surface = CVoronoiVolume(arInputPoints, arColorField, aVMin, aVMax, aNoiseDisplacement);
        surface.CreateSurface();
        CMesh mesh = surface.MaskMesh(arMask, arLabel, aOpacity);
        if(arOutputFilePath != ""){
            mesh.Export(arOutputFilePath);
        }
        return CModel(mesh);
    }

    CModel vivifyMesh(const std::vector<CPoint> &arInputPoints, const std::vector<normal_float> &arSurfaceField, normal_float aSurfaceThreshold,
                      const std::string &arOutputFilePath, std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax,
                      const string& arLabel, normal_float aOpacity, coord_t aNoiseDisplacement){
        //checks
        auto minmax = std::minmax_element(arSurfaceField.begin(), arSurfaceField.end());
        if (aSurfaceThreshold <= *minmax.first || aSurfaceThreshold >= *minmax.second){
            Log(LOG_ERROR, SURFACE_THRESHOLD_OUT_OF_RANGE);
        }
        std::vector<bool> mask;
        mask.reserve(arSurfaceField.size());
        for(const auto it: arSurfaceField){
            mask.emplace_back(it>aSurfaceThreshold);
        }
        return vivifyMesh(arInputPoints, mask, arOutputFilePath, arColorField, aVMin, aVMax, arLabel, aOpacity,
                          aNoiseDisplacement);
    }


    CModel vivifyModel(const std::vector<CPoint> &arInputPoints, const std::vector<std::vector<bool>> &arMasks, const std::string &arOutputFilePath,
                    std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                    vector<normal_float> &arOpacity, coord_t aNoiseDisplacement){
        //check for empty aOpacity
        if(arOpacity.size() == 0){
            arOpacity.resize(arMasks.size());
            for(size_t i = 0; i != arMasks.size(); i++){
                arOpacity.emplace_back((1/(arOpacity.size()+1))*(i+1));
            }
        }
        if(arOpacity.size()!= arMasks.size()){
            Log(LOG_ERROR, ARRAYS_NOT_EQUAL);
        }

        CVoronoiVolume surface = CVoronoiVolume(arInputPoints, arColorField, aVMin, aVMax, aNoiseDisplacement);
        surface.CreateSurface();
        CModel output_model = CModel();
        for(size_t i = 0; i != arMasks.size(); i++){
            output_model.AddMesh(surface.MaskMesh(arMasks[i], arLabel, arOpacity[i]));
        }
        if(arOutputFilePath != ""){
            output_model.Export(arOutputFilePath);
        }
        return output_model;
    }

    CModel vivifyModel(const std::vector<CPoint> &arInputPoints, const std::vector<normal_float> &arSurfaceField, vector<normal_float> &arSurfaceThresholds,
                       const std::string &arOutputFilePath, std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                       vector<normal_float> &arOpacity, coord_t aNoiseDisplacement){
        //checks
        auto minmax = std::minmax_element(arSurfaceField.begin(), arSurfaceField.end());
        for(const auto Threshold : arSurfaceThresholds){
            if (Threshold <= *minmax.first || Threshold >= *minmax.second){
                Log(LOG_ERROR, SURFACE_THRESHOLD_OUT_OF_RANGE);
            }
        }

        std::vector<std::vector<bool>> masks = vector<vector<bool>>(arSurfaceThresholds.size());
        for(size_t i = 0; i != arSurfaceThresholds.size(); i++){
            masks[i].reserve(arSurfaceField.size());
            for(const auto it: arSurfaceField){
                masks[i].emplace_back(it>arSurfaceThresholds[i]);
            }
        }

        return vivifyModel(arInputPoints, masks, arOutputFilePath, arColorField, aVMin, aVMax, arLabel, arOpacity,
                           aNoiseDisplacement);
    }
}