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
        setSize(sideLength, sideLength);
    }
    ~DragComponent() override
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
    int sideLength;
private:
    bool firstDragLoop = true;
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
        dragger.setAlwaysOnTop(true);
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
    void setColor(juce::Colour color)
    {
        backgroundColor = color;
    }
    void paint(juce::Graphics &g) override
    {
        g.fillAll(backgroundColor);
        g.setColour(juce::Colours::black);
        juce::Rectangle<int> draggerBox = dragger.getBounds();
        g.fillRect(draggerBox);
    }
    //returns the x position of the dragger as a percentage of its range
    float getXValueFloat()
    {
        float usableWidth = (float)width  - dragger.sideLength;
        float x = dragger.getCenterX();
        return x / usableWidth;
    }
    float getYValueFloat()
    {
        float usableHeight = (float)height  - dragger.sideLength;
        float y = dragger.getCenterY();
        return y / usableHeight;
    }
private:
    int x, y, width, height;
    DragComponent dragger;
    juce::Colour backgroundColor;
};






