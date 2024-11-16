#pragma once
#include "IElement.h"

namespace GeomSolver
{
    template<class T>
    class Circle : public IElement<T>
    {
    public:
        template<class... V>
        Circle(long long id, std::string& name, const std::tuple<V...>& arg) :IElement<T>(id, name)
        {
            m_origin.x = static_cast<T>(std::get<0>(arg));
            m_origin.y = static_cast<T>(std::get<1>(arg));
            radius = static_cast<T>(std::get<2>(arg));
            IElement<T>::m_pts.push_back(m_origin);

            IElement<T>::m_bounds[0] = static_cast<T>(std::get<0>(arg) - std::get<2>(arg));
            IElement<T>::m_bounds[1] = static_cast<T>(std::get<0>(arg) + std::get<2>(arg));
            IElement<T>::m_bounds[2] = static_cast<T>(std::get<1>(arg) + std::get<2>(arg));
            IElement<T>::m_bounds[3] = static_cast<T>(std::get<1>(arg) - std::get<2>(arg));
            IElement<T>::m_type = ElementType::Circle;
        }

        bool isOverlap(const IElement<T>* elm, const T epsilon = 0.005) override
        { 
            if (elm == nullptr) return false;

            switch (elm->getType())
            {
            case ElementType::Rectangle:
            case ElementType::Triangle:
            {
                if (!separatingAxisTheoremSingleElement(elm))
                {
                    return false;
                }
                if (!elm->separatingAxisTheoremSingleElement(this))
                {
                    return false;
                }
                return true;
            }
            case ElementType::Circle:
            {
                Circle<T>* tmp = (Circle<T>*)elm;
                RectCoordinate<T>axis1 = m_origin - tmp->m_pts[0];
                // r1+r2 >c1-c2
                if(radius + tmp->getRadius() > axis1.norm())
                {
                    return true;
                }
                return false;

            }
            default:
            {
                LOGE("Unknown ElementType %d in %s",static_cast<int>(elm->getType()), elm->getName().c_str());
            }
            }
            return false;
        };
        bool isInterset(const RectCoordinate<T>& coord, const T epsilon = 0.005) const override
        { 
            auto temp = m_origin - coord;
            if (abs(temp.norm()-radius) < epsilon)
            {
                return true;
            }
            return false; 
        }

        T minDistance(const RectCoordinate<T>& coord) const override
        {
            auto d1 = RectCoordinate<T>::distanceToPoint(m_origin, coord);
            return d1;
        };

        void projectionVectorGeneration() override
        {
            // there is no unique projections, need create each comparisson
        }

        bool separatingAxisTheoremSingleElement(const IElement<T>* element) const override
        {
            RectCoordinate<T>axis = m_origin - element->getPts()[0];
            T minNorm = axis.norm();
            for (size_t i = 0; i < element->getPts().size(); i++)
            {
                RectCoordinate<T> axis1 = m_origin - element->getPts()[i];
                T norm = axis1.norm();
                if (minNorm > norm)
                {
                    axis = axis1;
                    minNorm = norm;
                }
            }
            axis.x = axis.x / minNorm;
            axis.y = axis.y / minNorm;
;
            // project both shapes onto the axis
            auto proj1 = this->project(axis);
            auto proj2 = element->project(axis);
                // do the projections overlap?
            if (!IElement<T>::overlap(proj1, proj2)) {
                return false;
            }
            return true;
        }

        RectCoordinate<T> project(RectCoordinate<T> axis) const override
        {
            T min = axis.dot(m_origin) -radius;
            T max = min +2*radius;
            return RectCoordinate<T>(min, max);
        }

        T getRadius() { return radius; }
    private:
        RectCoordinate<T> m_origin;
        T radius;
    };
} // namespace GeomSolver

