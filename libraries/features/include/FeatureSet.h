////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     FeatureSet.h (features)
//  Authors:  Chuck Jacobs
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Feature.h"
#include "DoubleVector.h"
#include "InputFeature.h"
#include "CoordinateList.h"
#include "Model.h"

#include <string>
#include <vector>
#include <memory>

namespace features
{    
    using DataVector = linear::DoubleVector; 

    class FeatureSet
    {
    public:
        FeatureSet();

        void Reset();
        bool ProcessInputData(const DataVector& inData) const; // returns true if we generated output (in which case, call GetOutput())
        // TODO: to deal with buffering nodes (e.g., FFT), need to split ProcessInputData into 2 or 3 phases:
        // SetInput(...) // triggers dirty calc cascade
        // GetValue() // sets dirty bit off
        bool HasOutput() const; // Returns dirty bit value. If we have multiple output features, then this should just be a function on the feature
        DataVector GetOutput() const;
        
        std::shared_ptr<InputFeature> GetInputFeature() const;
        std::shared_ptr<Feature> GetOutputFeature() const;
        void SetOutputFeature(const std::shared_ptr<Feature>& output);
        std::shared_ptr<Feature> GetFeature(const std::string& featureId) const;
        std::vector<std::string> GetFeatureIds() const;

        template <typename FeatureType, typename... Args>
        std::shared_ptr<FeatureType> CreateFeature(Args... args);

        // Function for creating a feature from a string description
        std::shared_ptr<Feature> CreateFeatureFromDescription(const std::vector<std::string>& description);

        void Deserialize(std::istream& inStream);
        void Serialize(std::ostream& outStream) const; // Serializes all features in the graph
        void SerializeActiveGraph(std::ostream& outStream) const; // Serializes only the features needed to recreate output

        template <typename Visitor>
        void Visit(Visitor& visitor) const; // Visits all features in the graph
        template <typename Visitor>
        void VisitActiveGraph(Visitor& visitor) const; // Visits only the features needed to recreate output

        /// <summary> Adds the feature set to a model. </summary>
        ///
        /// <param name="map"> [in,out] The model. </param>
        /// <param name="inputCoordinates"> The input coordinates. </param>
        /// <returns> The output coordinates </returns>
        layers::CoordinateList AddToModel(layers::Model& model, const layers::CoordinateList& inputCoordinates) const;

    private:
        FeatureMap _featureMap;
        std::shared_ptr<InputFeature> _inputFeature;
        std::shared_ptr<Feature> _outputFeature;
        mutable size_t _numItemsProcessed = 0;
    };
}

#include "../tcc/FeatureSet.tcc"
