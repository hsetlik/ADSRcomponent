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
                decaySlider(30, 30, 135, 100, 20),
                releaseSlider(165, 30, 135, 100, 20)
                
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decaySlider);
        addAndMakeVisible(releaseSlider);
        
        setBounds(0, 0, 400, 300);
        decaySlider.resetPointHome(25, 50);
        releaseSlider.resetPointHome(25, 50);
        
        decaySlider.setChildColor(juce::Colours::orange);
        releaseSlider.setChildColor(juce::Colours::green);
        decaySlider.addPeer(&attackDragger, left);
        decaySlider.addPeer(&releaseSlider, top);
        releaseSlider.addPeer(&decaySlider, top);
        
        
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
private:
    DraggerContainer attackDragger;
    DraggerContainer decaySlider;
    DraggerContainer releaseSlider;

};
