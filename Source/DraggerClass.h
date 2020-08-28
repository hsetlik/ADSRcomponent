#pragma once

#include <JuceHeader.h>
#include <stdio.h>

class DragComponent : public juce::Component
{
public:
    DragComponent()
    {
        int initSideLength = getParentHeight();
        sideLength = initSideLength;
        setBounds(0, 0, sideLength, sideLength);
        auto bounds = getLocalBounds().reduced(20);
        constrainer.setSizeLimits(bounds.getY(),
                                  bounds.getX(),
                                  bounds.getWidth(),
                                  bounds.getHeight());
        constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
        printf("Start X: %d\n", getX());
        printf("Start Y: %d\n", getY());
        setTopLeftPosition(getX(), getY() + sideLength);
    }
    ~DragComponent() override
    {
    }
    void moved() override
    {
    
    }
    void resized() override
    {
        sideLength = getParentHeight();
        setSize(sideLength, sideLength);
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::blue);
    }
    void mouseDown(const juce::MouseEvent &event) override
    {
        dragger.startDraggingComponent(this, event);
        printf("Second X: %d\n", getX());
        printf("Second Y: %d\n", getY());
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
    dragger.dragComponent(this, event, &constrainer);
    }
    
private:
    bool firstDragLoop = true;
    int sideLength;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
};

class DragRangeRect : public juce::Component
{
public:
    DragRangeRect(int ix, int iy, int iwidth, int iheight)
    {
        addAndMakeVisible(dragger);
        
        x = ix;
        y = iy;
        width = iwidth;
        height = iheight;
        setBounds(x, y, width, height);
        dragger.setBounds(x, y, height, height);
    }
    ~DragRangeRect()
    {}
    void reInit(int ix, int iy, int iwidth, int iheight)
    {
        x = ix;
        y = iy;
        width = iwidth;
        height = iheight;
        setBounds(x, y, width, height);
    }
    void paint(juce::Graphics &g) override
    {
        //g.fillAll(juce::Colours::white);
    }
private:
    int x, y, width, height;
    DragComponent dragger;
};






