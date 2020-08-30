//
//  SustainDragger.h
//  ADSRcomponent - App
//
//  Created by Hayden Setlik on 8/30/20.
//

#pragma once
#include <JuceHeader.h>
#include "DraggerClass.h"

class SustainDragger : public juce::Component
{
public:
    SustainDragger(int heightArg)
    {
        height = heightArg;
        setSize(getParentWidth(), height);
        juce::Rectangle<int> bounds = getBoundsInParent().reduced(5);
        constrainer.setSizeLimits(bounds.getY(),
                                  bounds.getX(),
                                  bounds.getWidth(),
                                  bounds.getHeight());
        constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
        setTopLeftPosition(0, 0);
    }
    ~SustainDragger() {}
    void mouseDown(const juce::MouseEvent &event) override
    {
        dragger.startDraggingComponent(this, event);
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        dragger.dragComponent(this, event, &constrainer);
    }
    void assignColor(juce::Colour inputColor)
    {
        setColor = inputColor;
    }
    int centerX() {return getX() + (getWidth() / 2);}
    int centerY() {return getY() + (getHeight() / 2);}
private:
    
    int height;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::Colour setColor = juce::Colours::blue;
    
};
//======================================================

class SustainContainer : public juce::Component, public juce::ComponentListener
{
private:
    SustainDragger dragger;
    DragPoint* leftNeighbor;
    DragPoint* rightNeighbor;
    int lastMinX, lastMaxX; //note: these need to be set such that the SustainContainer does not overlap the neighbor points
public:
    SustainContainer(DragPoint* decayPoint, DragPoint* releasePoint) : dragger(25)
    {
        leftNeighbor = decayPoint;
        rightNeighbor = releasePoint;
        leftNeighbor->addComponentListener(this);
        rightNeighbor->addComponentListener(this);
        resetLastValues();
    }
    ~SustainContainer() {}
    void resetLastValues()
    {
        lastMinX = leftNeighbor->getRight();
        lastMaxX = rightNeighbor->getX();
    }
};
