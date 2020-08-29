#pragma once

#include <JuceHeader.h>
#include <stdio.h>

class DragPointComponent : public juce::Component
{
public:
    DragPointComponent()
    {
        int initSideLength = getParentHeight();
        sideLength = initSideLength;
        setSize(sideLength, sideLength);
        float parentSideRatio = getParentWidth() / getParentHeight();
        setBoundsRelative(0.0f, 0.0f, parentSideRatio, 1.0f);
        juce::Rectangle<int> bounds = getBoundsInParent().reduced(5);
        constrainer.setSizeLimits(bounds.getY(),
                                  bounds.getX(),
                                  bounds.getWidth(),
                                  bounds.getHeight());
        constrainer.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);
        setTopLeftPosition(0, 0);
    }
    ~DragPointComponent() override
    {
    }
    void moved() override
    {
    
    }
    int getCenterX()
    {
        return getX() + (sideLength / 2);
    }
    int getCenterY()
    {
        return getY() + (sideLength / 2);
    }
    void resized() override
    {
        sideLength = getParentHeight();
        setSize(sideLength, sideLength);
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(setColor);
    }
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
    
private:
    bool firstDragLoop = true;
    int sideLength;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::Colour setColor = juce::Colours::blue;
};

class DragPointContainer : public juce::Component
{
public:
    DragPointContainer(int ix, int iy, int iwidth, int iheight)
    {
        addAndMakeVisible(dragger);
        
        x = ix;
        y = iy;
        width = iwidth;
        height = iheight;
        setBounds(x, y, width, height);
        dragger.setBounds(x, y, height, height);
        dragger.setTopLeftPosition(0, 0);
    }
    ~DragPointContainer()
    {}
    void reInit(int ix, int iy, int iwidth, int iheight)
    {
        x = ix;
        y = iy;
        width = iwidth;
        height = iheight;
        setBounds(x, y, width, height);
        dragger.setBounds(x, y, height, height);
        dragger.setTopLeftPosition(0, 0);
        setTopLeftPosition(x, y);
    }
    void setNodeColor(juce::Colour inputColor)
    {
        dragger.assignColor(inputColor);
    }
    void paint(juce::Graphics &g) override
    {
        g.fillAll(juce::Colours::white);
    }
    int getXSetting()
    {
        printf ("X set to: %d\n", dragger.getCenterX() - x);
        return (dragger.getCenterX() - x);
    }
private:
    int x, y, width, height;
    DragPointComponent dragger;
};






