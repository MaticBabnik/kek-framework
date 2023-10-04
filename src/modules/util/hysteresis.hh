#pragma once

template <typename T> class HysteresisController
{
  protected:
    bool state;
    T hyst;
    T target;

  public:
    HysteresisController(T hyst, T target);

    bool act(T in);
    void setTarget(T target);
};
