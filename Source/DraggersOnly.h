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
    ADSRenv() : attackDragger(30, 30, 80, 20),
                decaySlider(30, 50, 100, 20)
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decaySlider);
        setBounds(0, 0, 400, 300);
        
        decaySlider.setChildColor(juce::Colours::orange);
        //now to set up the limits for cont2
        decaySlider.addPeer(&attackDragger, left);
        
        
        
       
    }
    ~ADSRenv()
    {
    }
    void resized() override
    {
        attackDragger.resized();
        decaySlider.resized();
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        printf("Mouse Dragging\n");
        decaySlider.checkLimitUpdates();
    }
private:
    DraggerContainer attackDragger, decaySlider;
    
};
