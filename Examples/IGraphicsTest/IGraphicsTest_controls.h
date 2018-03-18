#define LERP(a,b,f) ((b-a)*f+a)

class IVMappingControl: public IControl
{
public:
  IVMappingControl(IDelegate& dlg, IRECT bounds, int nSliders)
  : IControl(dlg, bounds)
  , mNSliders(nSliders)
  {
    Clamp(-1., 1.);

    float sliderWidth = mRECT.W() / (float) mNSliders;

    mSteps = new float[mNSliders];
    mSliderBounds = new IRECT[mNSliders];
    
    for(auto i=0; i<mNSliders; i++)
    {
      int lpos = (i * sliderWidth);
      mSteps[i] = (float) i/((float)mNSliders-1.);
      mSliderBounds[i] = IRECT(mRECT.L + lpos , mRECT.T, mRECT.L + lpos + sliderWidth, mRECT.B);
    }
  }
  
  virtual ~IVMappingControl()
  {
    delete [] mSteps;
    delete [] mSliderBounds;
  }
  
  void Draw(IGraphics& g) override
  {
    float knotsX[mNSliders];
    float knotsY[mNSliders];
    
    float xStart = mRECT.L;
    float yStart = mRECT.B;
    
//    g.PathStart();
    
    for(auto i=0; i<mNSliders; i++)
    {
      float yPos = mSteps[i] * mRECT.H();
      float top = mRECT.B - yPos;
      IRECT srect = IRECT(mSliderBounds[i].L, top, mSliderBounds[i].R, mRECT.B).GetMidHPadded(4);
      g.FillRect(COLOR_BLUE, srect);
      
      float xEnd = knotsX[i] = mSliderBounds[i].MW();
      float yEnd = knotsY[i] = top;
//
//      g.PathLine(xStart, yStart, xEnd, yEnd);
      
//      xStart = xEnd;
//      yStart = yEnd;
    }
    
//    g.PathLine(xStart, yStart, mRECT.R, mRECT.B);
//    g.PathLine(mRECT.R, mRECT.B, mRECT.L, mRECT.B);
//
//    g.PathFill(COLOR_BLACK);
    
    //curve
#if 1
    float px1[mNSliders];
    float px2[mNSliders];

    ComputeControlPoints(knotsX, px1, px2);

    float py1[mNSliders];
    float py2[mNSliders];

    ComputeControlPoints(knotsY, py1, py2);
    
    g.PathStart();

    g.PathMoveTo(knotsX[0], knotsY[0]);

    for (auto i = 0; i < mNSliders-1; ++i)
    {
      g.PathCurveTo(px1[i], py1[i], px2[i], py2[i], knotsX[i+1], knotsY[i+1]);
    }

    g.PathStroke(COLOR_GREEN, 2.);
#endif

    //line
    g.PathStart();
    xStart = knotsX[0];
    yStart = knotsY[0];

    for (auto i = 1; i < mNSliders; ++i)
    {
      float xEnd = knotsX[i];
      float yEnd = knotsY[i];

      g.PathLine(xStart, yStart, xEnd, yEnd);

      xStart = xEnd;
      yStart = yEnd;
    }

    g.PathStroke(COLOR_BLACK, 2.);
    
//    g.DrawLine(COLOR_BLACK, mRECT.L + (mLUValueNorm * mRECT.W()),  mRECT.T, mRECT.L + (mLUValueNorm * mRECT.W()), mRECT.B, 0, 2.);
//    g.DrawLine(COLOR_BLACK, mRECT.L, mRECT.B - (mOutputValueNorm * mRECT.H()), mRECT.R, mRECT.B - (mOutputValueNorm * mRECT.H()), 0, 2.);
  }

  virtual void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (mod.S)
    {
      SnapToMouse(x, y, 0, 0, true);

      return;
    }

    mPrevSliderHit = -1;

    SnapToMouse(x, y, 0, 0);
  }
//
//  virtual void OnMouseUp(float x, float y, const IMouseMod& mod) override
//  {
//
//  }
//
  virtual void OnMouseDrag(float x, float y, float dX, float dY, const IMouseMod& mod) override
  {
    SnapToMouse(x, y, dX, dY);
  }
//
//  virtual void OnMouseDblClick(float x, float y, const IMouseMod& mod) override
//  {
//  }
//
//  virtual void OnMouseWheel(float x, float y, const IMouseMod& mod, float d) override
//  {
//
//  };
//
//  virtual bool OnKeyDown(float x, float y, int key) override
//  {
//    return false;
//  }
  
  
//  float* GetState();
//  void SetState(float* data);
//  void SetLinear();
//  void SetMax();
//  void MoveLeft();
//  void MoveRight();
//  void MoveUp();
//  void MoveDown();
//  void SetCurve(float curve);
//  int LookUp(int value);

private:
  void ComputeControlPoints(float K[], float p1[], float p2[])
  {
    auto n = mNSliders-1;
    
    /*rhs vector*/
    float a[mNSliders];
    float b[mNSliders];
    float c[mNSliders];
    float r[mNSliders];

    /*left most segment*/
    a[0]=0.;
    b[0]=2.;
    c[0]=1.;
    r[0] = K[0]+2.*K[1];
    
    /*internal segments*/
    for (auto i = 1; i < n - 1; i++)
    {
      a[i]=1.;
      b[i]=4.;
      c[i]=1.;
      r[i] = 4. * K[i] + 2. * K[i+1];
    }
    
    /*right segment*/
    a[n-1]=2.;
    b[n-1]=7.;
    c[n-1]=0.;
    r[n-1] = 8.*K[n-1]+K[n];
    
    /*solves Ax=b with the Thomas algorithm (from Wikipedia)*/
    for (auto i = 1; i < n; i++)
    {
      auto m = a[i]/b[i-1];
      b[i] = b[i] - m * c[i - 1];
      r[i] = r[i] - m*r[i-1];
    }
    
    p1[n-1] = r[n-1]/b[n-1];
    for (auto i = n - 2; i >= 0; --i)
      p1[i] = (r[i] - c[i] * p1[i+1]) / b[i];
    
    /*we have p1, now compute p2*/
    for (auto i=0;i<n-1;i++)
      p2[i]=2.*K[i+1]-p1[i+1];
    
    p2[n-1]=0.5*(K[n]+p1[n-1]);
  }
  
  void SnapToMouse(float x, float y, float dX, float dY, bool shiftClicked  = false)
  {
    mRECT.Constrain(x, y);

    float yValue = (y-mRECT.T) / mRECT.H();

    yValue = std::round( yValue / mGrain ) * mGrain;

    int sliderTest = -1;

    for(auto i = 0; i < mNSliders; i++)
    {
      if(mSliderBounds[i].Contains(x, y))
      {
        sliderTest = i;
        break;
      }
    }

    if (sliderTest > -1)
    {
      mSteps[sliderTest] = 1. - BOUNDED(yValue, 0., 1.);
      mSliderHit = sliderTest;

      if (mPrevSliderHit != -1)
      {
        if (abs(mPrevSliderHit - mSliderHit) > 1 || shiftClicked)
        {
          int lowBounds, highBounds;

          if (mPrevSliderHit < mSliderHit)
          {
            lowBounds = mPrevSliderHit;
            highBounds = mSliderHit;
          }
          else
          {
            lowBounds = mSliderHit;
            highBounds = mPrevSliderHit;
          }

          for (auto i = lowBounds; i < highBounds; i++)
          {
            float frac = (float)(i - lowBounds) / float(highBounds-lowBounds);
            mSteps[i] = LERP(mSteps[lowBounds], mSteps[highBounds], frac);
          }
        }
      }
      mPrevSliderHit = mSliderHit;
    }
    else
    {
      mSliderHit = -1;
    }

    SetDirty();
  }
  
  float* mSteps;

  int mNSliders;
  int mPrevSliderHit = -1;
  int mSliderHit = -1;
  float mGrain = 0.001f;
  float mLUValueNorm = 0.5;
  float mOutputValueNorm = 0.5;
//  int mMouseDownX = 0;
//  float mNumDragLeftHits;

  IRECT *mSliderBounds;
};

#define OLLERP(a,b,f) ((b-a)*f+a)



//

//
//void IVMappingControl::OnMouseUp(int x, int y, IMouseMod* pMod)
//{
//  SendChangesToProcessor();
//}
//
//void IVMappingControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod)
//{
//  SnapToMouse(x, y, dX, dY);
//}
//

//
//float* IVMappingControl::GetState()
//{
//  return mSteps;
//}
//
//void IVMappingControl::SetState(float* data)
//{
//  memcpy(mSteps, data, mNumSliders * sizeof(float));
//
//  SetDirty();
//}
//
//void IVMappingControl::SetLinear()
//{
//  for(auto i=0; i<mNumSliders; i++)
//  {
//    mSteps[i] = (float) i/((float)mNumSliders-1.);
//  }
//
//  SetDirty();
//}
//
//void IVMappingControl::SetMax()
//{
//  for(auto i=0; i<mNumSliders; i++)
//  {
//    mSteps[i] = 1.;
//  }
//
//  SetDirty();
//}
//
//void IVMappingControl::MoveLeft()
//{
//  float newArray[mNumSliders];
//  memset(newArray, 0, mNumSliders * sizeof(float));
//
//  int regionToCopy = mNumSliders - 5;
//  regionToCopy = BOUNDED(regionToCopy, 5, mNumSliders);
//  memcpy(newArray, mSteps + (mNumSliders-regionToCopy), regionToCopy * sizeof(float));
//  memcpy(mSteps, newArray, mNumSliders * sizeof(float));
//
//  SetDirty();
//}
//
//void IVMappingControl::MoveRight()
//{
//  float newArray[mNumSliders];
//  memset(newArray, 0, mNumSliders * sizeof(float));
//  memcpy(newArray + 5, mSteps, (mNumSliders - 5) * sizeof(float));
//  memcpy(mSteps, newArray, mNumSliders * sizeof(float));
//
//  SetDirty();
//}
//
//void IVMappingControl::MoveUp()
//{
//  for (int i=0; i<mNumSliders; i++)
//  {
//    mSteps[i] = BOUNDED(mSteps[i] + 0.05, 0., 1.);
//  }
//
//  SetDirty();
//}
//
//void IVMappingControl::MoveDown()
//{
//  for (int i=0; i<mNumSliders; i++)
//  {
//    mSteps[i] = BOUNDED(mSteps[i] - 0.05, 0., 1.);
//  }
//
//  SetDirty();
//}
//
//void IVMappingControl::SetCurve(float curve)
//{
//  curve = BOUNDED(curve, -0.999, 0.98);
//
//  if (curve > 0.)
//  {
//    curve = 1.-curve;
//  }
//  else
//  {
//    curve = 1. + (curve * -10.);
//  }
//
//  for (int i=0; i<mNumSliders; i++)
//  {
//    mSteps[i] = powf((float) i/((float)mNumSliders-1.), curve);
//  }
//
//  SetDirty();
//}
//
//int IVMappingControl::LookUp(int value)
//{
//  mLUValueNorm = (float) value / 127.;
//  SetDirty(false);
//  mOutputValueNorm = mSteps[int(round(mLUValueNorm * (mNumSliders-1)))];
//  return int(mOutputValueNorm * 127.);
//}


