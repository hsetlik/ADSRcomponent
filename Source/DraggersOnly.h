//
//  DraggersOnly.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/28/20.
//

#pragma once
#include <JuceHeader.h>
#include "DraggerClass.h"

class DraggerGroup : public juce::Component
{
public:
    DraggerGroup() : cont1(30, 30, 200, 20),
                     cont2(30, 50, 200, 20)
    {
        //put the two sliders on and change their color to tell them apart
        addAndMakeVisible(cont1);
        addAndMakeVisible(cont2);
        setBounds(0, 0, 400, 300);
        
        cont2.setChildColor(juce::Colours::orange);
        //now to set up the limits for cont2
        cont2.addPeer1(&cont1, DraggerContainer::left);
        
        
        
       
    }
    ~DraggerGroup()
    {
    }
    void resized() override
    {
        cont1.resized();
        cont2.resized();
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        printf("Mouse Dragging\n");
        cont2.checkLimitUpdates();
    }
private:
    DraggerContainer cont1, cont2;
    
};
