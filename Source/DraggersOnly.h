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
    ADSRenv() : attackDragger(30, 30, 80, 20, hor),
                decayDragger(30, 50, 100, 20, hor),
                sustainDragger(130, 0, 80, 300, vert)
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decayDragger);
        addAndMakeVisible(sustainDragger);
        setBounds(0, 0, 400, 300);
        
        decayDragger.setChildColor(juce::Colours::orange);
        sustainDragger.setChildColor(juce::Colours::pink);
        //now to set up the limits for cont2
        decayDragger.addPeer(&attackDragger, left);
        decayDragger.addPeer(&sustainDragger, top);
        decayDragger.addPeer(&sustainDragger, bottom);
        
        
        
       
    }
    ~ADSRenv()
    {
    }
    void resized() override
    {
        attackDragger.resized();
        decayDragger.resized();
        sustainDragger.resized();
        
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        printf("Mouse Dragging\n");
        decayDragger.checkLimitUpdates();
    }
private:
    DraggerContainer attackDragger, decayDragger, sustainDragger;
    
};
