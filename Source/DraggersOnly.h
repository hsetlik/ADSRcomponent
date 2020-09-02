//
//  DraggersOnly.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/28/20.
//

#pragma once
#include <JuceHeader.h>
#include "DraggerClass.h"

class ADSRenv : public juce::Component, public juce::ComponentListener
{
public:
    ADSRenv() : attackDragger(30, 30, 80, 20, 20),
                decayDragger(30, 50, 135, 100, 20),
                releaseDragger(165, 50, 135, 100, 20)
                
    {
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decayDragger);
        addAndMakeVisible(releaseDragger);
        decayDragger.addComponentListener(this);
        releaseDragger.addComponentListener(this);
        
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
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override
    {
        decayDragger.checkLimitUpdates();
        releaseDragger.checkLimitUpdates();
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
