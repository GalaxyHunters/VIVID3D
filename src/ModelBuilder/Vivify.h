#ifndef VIVID_VIVIFY_H
#define VIVID_VIVIFY_H

#include "Model.h"
#include "VoronoiVolume.h"
#include "LogFile.h"
namespace vivid {

    /**
     * vivify one function to make a model - Mask version
     * @param[in] aOutputPath Path and name for output file
     * @param[in] arInputPoints the input point data in x,y,z form.
     * @param[in] arMask a boolean mask of true and false points
     * @param[in] arColorField a vector containing the color field of each point
     * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
     * @param[in] aVMax the maximum value in arColorField, anything below will be set to aVMax
     * @param[in] arLabel the label to assign to the new mesh
     * @param[in] aOpacity the alpha to assign to the new mesh
     * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
     */
    CModel vivifyMesh(const std::vector<CPoint> &arInputPoints, const std::vector<bool> &arMask, const std::string &arOutputFilePath,
                   std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                   normal_float aOpacity, coord_t aNoiseDisplacement);

    /**
     * vivify one function to make a model - threshold version
     * @param[in] aOutputPath Path and name for output file
     * @param[in] arInputPoints the input point data in x,y,z form.
     * @param[in] arSurfaceField a vector containing Surface field for points
     * @param[in] aSurfaceThreshold Value Threshold for SurfaceField, this value will be the created surface
     * @param[in] arColorField a vector containing the color field of each point
     * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
     * @param[in] aVMax the maximum value in arColorField, anything below will be set to aVMax
     * @param[in] arLabel the label to assign to the new mesh
     * @param[in] aOpacity the alpha to assign to the new mesh
     * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
     */
    CModel vivifyMesh(const std::vector<CPoint> &arInputPoints, const std::vector<normal_float> &arSurfaceField, normal_float aSurfaceThreshold,
                      const std::string &arOutputFilePath, std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax,
                      const string& arLabel, normal_float aOpacity, coord_t aNoiseDisplacement);

    /**
     * vivify one function to make a model, with multiple masks - Mask version
     * @param[in] aOutputPath Path and name for output file
     * @param[in] arInputPoints the input point data in x,y,z form.
     * @param[in] arMask an array of boolean masks of true and false points
     * @param[in] arColorField a vector containing the color field of each point
     * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
     * @param[in] aVMax the maximum value in arColorField, anything below will be set to aVMax
     * @param[in] arLabel the label to assign to the new mesh
     * @param[in] aOpacity the alpha to assign to the new mesh
     * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
     */
    CModel vivifyModel(const std::vector<CPoint> &arInputPoints, const std::vector<std::vector<bool>> &arMasks, const std::string &arOutputFilePath,
                       std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax, const string& arLabel,
                       vector<normal_float> &arOpacity, coord_t aNoiseDisplacement);

    /**
     * vivify one function to make a model, with multiple masks - threshold version
     * @param[in] aOutputPath Path and name for output file
     * @param[in] arInputPoints the input point data in x,y,z form.
     * @param[in] arSurfaceField a vector containing Surface field for points
     * @param[in] arSurfaceThresholds a vector of Threshold values for SurfaceField, this value will be the created surfaces
     * @param[in] arColorField a vector containing the color field of each point
     * @param[in] aVMin the minimum value in arColorField, anything below will be set to aVMin
     * @param[in] aVMax the maximum value in arColorField, anything below will be set to aVMax
     * @param[in] arLabel the label to assign to the new mesh
     * @param[in] aAlpha the alpha to assign to the new mesh
     * @param[in] aNoiseDisplacement the Voronoi algorithm struggles with equidistant point data, a small noise displacement improves algorithm speed
     */
    CModel vivifyModel(const std::vector<CPoint> &arInputPoints, const std::vector<normal_float> &arSurfaceField, vector<normal_float> &arSurfaceThresholds,
                       const std::string &arOutputFilePath, std::vector<normal_float> &arColorField, normal_float aVMin, normal_float aVMax,
                       const string& arLabel, vector<normal_float> &arOpacity, coord_t aNoiseDisplacement);
}

#endif //VIVID_VIVIFY_H
