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
private:
    DraggerContainer attackDragger, decayDragger, sustainDragger;

public:
    ADSRenv() : attackDragger(10, 10, 75, 12, hor),
                decayDragger(10, 22, 225, 12, hor),
                sustainDragger(10 + 225, 0, 225, 300, vert)
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(attackDragger);
        addAndMakeVisible(decayDragger);
        addAndMakeVisible(sustainDragger);
        setBounds(0, 0, 400, 300);
        
        decayDragger.setChildColor(juce::Colours::orange);
        sustainDragger.setChildColor(juce::Colours::pink);
        //sustainDragger.point.setHeightByInt(20);
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

};
