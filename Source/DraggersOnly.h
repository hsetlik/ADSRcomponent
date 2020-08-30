//
//  DraggersOnly.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/28/20.
//

#pragma once
#include <JuceHeader.h>
#include "DraggerClass.h"

class ADSRenv : public juce::Component
{
public:
    ADSRenv() : attackDragger(30, 30, 80, 20, 20),
                decayDragger(30, 30, 135, 100, 20),
                releaseDragger(165, 30, 135, 100, 20)
                
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decayDragger);
        addAndMakeVisible(releaseDragger);
        
        setBounds(0, 0, 400, 300);
        decayDragger.resetPointHome(25, 50);
        releaseDragger.resetPointHome(25, 50);
        
        decayDragger.setChildColor(juce::Colours::orange);
        releaseDragger.setChildColor(juce::Colours::green);
        decayDragger.addPeer(&attackDragger, left);
        decayDragger.addPeer(&releaseDragger, top);
        releaseDragger.addPeer(&decayDragger, top);
        
        pAttack = &attackDragger;
        pDecay = &decayDragger;
        pRelease = &releaseDragger;
    }
    ~ADSRenv()
    {
    }
    void resized() override
    {
       
        
        
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        //decayDragger.checkLimitUpdates();
    }
    DraggerContainer* pAttack;
    DraggerContainer* pDecay;
    DraggerContainer* pRelease;
private:
    DraggerContainer attackDragger;
    DraggerContainer decayDragger;
    DraggerContainer releaseDragger;
};
