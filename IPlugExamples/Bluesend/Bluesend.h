#ifndef __BLUESEND__
#define __BLUESEND__

#include "IPlug_include_in_plug_hdr.h"

class Bluesend : public IPlug
{
public:
  Bluesend(IPlugInstanceInfo instanceInfo);
  ~Bluesend();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double mGain;
};

#endif
