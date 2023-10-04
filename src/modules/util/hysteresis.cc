#include "hysteresis.hh"

template <typename T>
HysteresisController<T>::HysteresisController(T hyst, T target)
{
    this->state = false;
    this->hyst = hyst;
    this->target = target;
}

template <typename T> bool HysteresisController<T>::act(T in)
{
    if (this->state)
    {
        if (in > this->target + this->hyst)
        {
            this->state = false;
        }
    }
    else
    {
        if (in < this->target)
        {
            this->state = true;
        }
    }

    return this->state;
}

template <typename T> void HysteresisController<T>::setTarget(T target)
{
    this->target = target;
}
