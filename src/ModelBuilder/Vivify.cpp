#include "Vivify.h"

namespace vivid{
    int vivify_mesh(const std::string &arOutputFilePath, const std::vector<CPoint> &arInputPoints, const std::vector<bool> &arMask,
                    std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                    normal_float aOpacity, coord_t aNoiseDisplacement) {
        CVoronoiVolume surface = CVoronoiVolume(arInputPoints, arColorField, aVMin, aVMax, aNoiseDisplacement);
        surface.CreateSurface();
        CMesh mesh = surface.MaskMesh(arMask, arLabel, aOpacity);
        return mesh.Export(arOutputFilePath);
    }

    int vivify_mesh(const std::string &arOutputFilePath, const std::vector<CPoint> &arInputPoints, const std::vector<normal_float> &arSurfaceField,
                    normal_float aSurfaceThreshold, std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                    normal_float aOpacity, coord_t aNoiseDisplacement){
        //checks
        auto minmax = std::minmax_element(arSurfaceField.begin(), arSurfaceField.end());
        if (aSurfaceThreshold <= *minmax.first || aSurfaceThreshold >= *minmax.second){
            Log(LOG_ERROR, SURFACE_THRESHOLD_OUT_OF_RANGE);
        }
        std::vector<bool> mask = vector<bool>(arSurfaceField.size());
        for(auto it = arSurfaceField.begin(); it != arSurfaceField.end(); it++){
            mask.emplace_back(*it>aSurfaceThreshold);
        }
        return vivify_mesh(arOutputFilePath, arInputPoints, mask, arColorField, aVMin, aVMax, arLabel, aOpacity, aNoiseDisplacement);
    }


    int vivify_model(const std::string &arOutputFilePath, const std::vector<CPoint> &arInputPoints, const std::vector<std::vector<bool>> &arMasks,
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
        return output_model.Export(arOutputFilePath);
    }

    int vivify_model(const std::string &arOutputFilePath, const std::vector<CPoint> &arInputPoints, const std::vector<normal_float> &arSurfaceField,
                     vector<normal_float> &arSurfaceThresholds, std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                     vector<normal_float> &arOpacity, coord_t aNoiseDisplacement){
        //checks
        auto minmax = std::minmax_element(arSurfaceField.begin(), arSurfaceField.end());
        for(auto Threshold = arSurfaceThresholds.begin(); Threshold != arSurfaceThresholds.end(); Threshold++){
            if (*Threshold <= *minmax.first || *Threshold >= *minmax.second){
                Log(LOG_ERROR, SURFACE_THRESHOLD_OUT_OF_RANGE);
            }
        }

        std::vector<std::vector<bool>> masks = vector<vector<bool>>(arSurfaceThresholds.size());
        for(size_t i = 0; i != arSurfaceThresholds.size(); i++){
            masks[i] = vector<bool>(arSurfaceField.size());
            for(auto it = arSurfaceField.begin(); it != arSurfaceField.end(); it++){
                masks[i].emplace_back(*it>arSurfaceThresholds[i]);
            }
        }

        return vivify_model(arOutputFilePath, arInputPoints, masks, arColorField, aVMin, aVMax, arLabel, arOpacity, aNoiseDisplacement);
    }
}