#pragma once

#include "Effect.h"

class RadialBlur: public Effect {
private:
    const uint8_t pixels;
    FunctionGenerator fGenRotation = FunctionGenerator(FunctionGenerator::Sawtooth, 0.0f, 360.0f, 3.7f);

public:
    RadialBlur(uint8_t pixels) : pixels(pixels){}

    void ApplyEffect(IPixelGroup* pixelGroup){
        unsigned int pixelCount = pixelGroup->GetPixelCount();
        RGBColor* pixelColors = pixelGroup->GetColors();
        RGBColor* colorBuffer = pixelGroup->GetColorBuffer();

        float rotation = fGenRotation.Update();

        for (unsigned int i = 0; i < pixelCount; i++){
            unsigned int indexV = i, indexD = i;
            unsigned int tIndexV = 0, tIndexD = 0;
            bool validV = true, validD = true;

            uint16_t blurRange = uint16_t(Mathematics::Map(ratio, 0.0f, 1.0f, 1.0f, float(pixels / 2)));

            uint16_t R, G, B;

            R = (uint16_t)pixelColors[i].R;
            G = (uint16_t)pixelColors[i].G;
            B = (uint16_t)pixelColors[i].B;
            
            for (uint8_t j = 1; j < blurRange + 1; j++){
                validV = pixelGroup->GetRadialIndex(indexV, &tIndexV, blurRange, rotation);
                validD = pixelGroup->GetRadialIndex(indexD, &tIndexD, blurRange, rotation + 180.0f);

                indexV = tIndexV;
                indexD = tIndexD;

                if (validV && validD){
                    R = R + (uint16_t)pixelColors[indexV].R + (uint16_t)pixelColors[validD].R;
                    G = G + (uint16_t)pixelColors[indexV].G + (uint16_t)pixelColors[validD].G;
                    B = B + (uint16_t)pixelColors[indexV].B + (uint16_t)pixelColors[validD].B;
                }
            }
            
            colorBuffer[i].R = pixelColors[indexV].R;//Mathematics::Constrain(R / (blurRange * 2), 0, 255);
            colorBuffer[i].B = pixelColors[indexV].G;//Mathematics::Constrain(G / (blurRange * 2), 0, 255);
            colorBuffer[i].G = pixelColors[indexV].B;//Mathematics::Constrain(B / (blurRange * 2), 0, 255);
        }
        
        for (unsigned int i = 0; i < pixelCount; i++){
            pixelColors[i].R = colorBuffer[i].R;
            pixelColors[i].G = colorBuffer[i].G;
            pixelColors[i].B = colorBuffer[i].B;
        }
    }
};

