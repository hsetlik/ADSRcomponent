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
        addAndMakeVisible(dragger);
        leftNeighbor = decayPoint;
        rightNeighbor = releasePoint;
        leftNeighbor->addComponentListener(this);
        rightNeighbor->addComponentListener(this);
        dragger.addComponentListener(this);
        resetLastValues();
        int newWidth = lastMaxX - lastMinX;
        setBounds(lastMinX, 0, newWidth, lastMaxX);
        printf("for Sustain: %d, %d\n", lastMinX, newWidth);
    }
    ~SustainContainer() {}
    void paint(juce::Graphics &g) override
    {g.fillAll(juce::Colours::green);}
    void resetLastValues()
    {
        //lastMinX = (leftNeighbor->getScreenX()) + (leftNeighbor->getWidth());
        printf("right side: %d\n", lastMinX);
        lastMaxX = rightNeighbor->getScreenX();
        //printf("left side: %d\n", lastMaxX);
    }
    void getNewLeft()
    {
        if(leftNeighbor->getRight() != lastMinX)
        {
            int newMinX = (leftNeighbor->getScreenX()) + (leftNeighbor->getWidth());
            printf("new MinX: %d\n", newMinX);
            int newWidth = lastMaxX - newMinX;
            printf("new Width: %d\n", newWidth);
            setBounds(newMinX, 0, newWidth, getParentHeight());
        }
        resetLastValues();
        
    }
    void resized() override
    {
        dragger.resized();
    }
    void getNewRight()
    {
        if(rightNeighbor->getScreenX() != lastMaxX)
        {
            int newMaxX = rightNeighbor->getScreenX();
            int newWidth = newMaxX - lastMinX;
            setBounds(lastMinX, 0, newWidth, getParentHeight());
        }
        resetLastValues();
    }
    void setNeighborHeights()
    {
        
    }
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override
       {
           if(&component == leftNeighbor)
               getNewLeft();
           else if (&component == rightNeighbor)
               getNewRight();
           else if (&component == &dragger)
               setNeighborHeights();
       }
};
