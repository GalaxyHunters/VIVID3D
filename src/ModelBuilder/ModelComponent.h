#ifndef VIVID_MODELCOMPONENT_H
#define VIVID_MODELCOMPONENT_H

#include <string>
#include <functional>
#include <map>


#include "Point.h"
#include "Face.h"
#include "ColorMap.h"
#include "Material.h"
#include "LogFile.h"

namespace vivid
{
    enum FACE_TYPE {
        POINTS = 1, LINES = 2, TRIANGLES = 3, POLYGONS = 4,
    };

    /* FTrans_t is a function that changes CPoint to another CPoint */
    typedef std::function<const CPoint(const CPoint)> FTrans_t; // Note: no ref use here to avoid unpredictable behavior.

    /* Abstract Class defining 3D component for a model. Can be mesh, line, point cloud or anything else. */
    class CModelComponent
    {
    protected:
        vector<CPoint> mPoints = {};
        vector<CFace> mFaces = {};
        FACE_TYPE mElementType = POLYGONS;
        std::string mLabel = "";
        CColorMap mClm;
        CMaterial mMaterial;
        //Constructor, Copy Constructor, Destructor
        CModelComponent(){}
        CModelComponent(const FACE_TYPE aElementType) : mElementType(aElementType) {}
        CModelComponent(const normal_float aAlpha, const std::string &arLabel, const FACE_TYPE aObjType)
            : mLabel(arLabel), mElementType(aObjType), mClm(), mMaterial() { SetOpacity(aAlpha); }
    public:
        CModelComponent(const CModelComponent &arModel) : mPoints(arModel.mPoints), mFaces(arModel.mFaces), mLabel(arModel.mLabel), mElementType(arModel.mElementType), mClm(arModel.mClm), mMaterial(arModel.mMaterial) {}
        virtual ~CModelComponent() = default;;

        // Operator=
        inline CModelComponent& operator= (const CModelComponent& arModel) { mPoints=arModel.mPoints; mFaces=arModel.mFaces;
            mLabel=arModel.mLabel; mElementType=arModel.mElementType; mClm=arModel.mClm; mMaterial=arModel.mMaterial; return *this; }

        // Getters, Setters
        inline std::vector<CPoint> GetPoints() const { return mPoints; }
        inline size_t GetPointsCount() const { return mPoints.size(); }
        inline std::vector<CFace> GetFaces() const { return mFaces; }
        inline size_t GetFacesCount() const { return mFaces.size(); }
        inline std::string GetLabel() const { return mLabel; }
        inline normal_float GetOpacity() const { return mMaterial.GetOpacity(); }
        inline CColorMap GetColorMap() const { return mClm; }
        inline const CMaterial& GetMaterial() const { return mMaterial; }
        inline FACE_TYPE GetObjType() const { return mElementType; }

        inline void SetPoints(std::vector<CPoint> &arPoints) { mPoints = arPoints; }
        inline void SetFaces(std::vector<CFace> &arFaces) { mFaces = arFaces; }
        inline void SetLabel(const std::string &arLabel) { mLabel = arLabel; }
        inline void SetOpacity(normal_float aOpacity) { mMaterial.SetOpacity(aOpacity); }
        inline void SetColor(const string& arColor) { mClm = CColorMap(arColor); }
        inline void SetColorMap(const CColorMap& arColorMap) { mClm = arColorMap; }
        inline void SetColorMap(const PyColorMap& arColorMap) { mClm = arColorMap; }
        inline void SetMaterial(const CMaterial& arMaterial) { mMaterial = arMaterial; }

        /**
         * transform CMesh points by transformation function
         * @param[in] aTrans is a function(CPoint) -> CPoint
         */
        void TransformMesh(FTrans_t const &arTrans);
        /**
         * transform CMesh points by transformation matrix
         * @param[in] aTrans a 3x3 dimension matrix.
         */
        void TransformMesh(const array<CPoint, 3>& aTrans);
        /**
         * Rotate the CMesh points around a normal vector by an angel, counterclockwise
         * @param[in] arNormVec the x,y.z normal to rotate around.
         * @param[in] aRadAngel the angel to rotate by (in radians).
         */
        void RotateMesh(const CPoint &arNormVec, double aRadAngel);
        /**
         * Change CMesh points location by addition of x,y,z.
         * @param[in] arDirectionVec the x,y.z direction to move by it.
         */
        void MoveMesh(const CPoint &arDirectionVec);
        /**
         * Change CMesh points scale in x,y,z direction by x,y,z of the scale vector
         * @param[in] arScaleVec the x,y.z direction to move by it.
         */
        void ScaleMesh(const CPoint &arScaleVec);

        void ExportToObj(const std::string &arOutputFilePath, bool WithTexture = 1);

        /**
        * Assimp export. writes arModel in aFileType format at aOutputPath
        * @param[in] aOutputPath Path and name for output file
        * @param[in] aFileType 3D filetype format to write to (out of supported options)
        */
        int Export(const std::string &arOutputFilePath, const std::string& arFileType = "gltf2");
    };
};     // namespace vivid
#endif // VIVID_MODELCOMPONENT_H