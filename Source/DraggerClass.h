#pragma once
#include <JuceHeader.h>

#include <stdio.h>

class DragPoint : public juce::Component
{
public:
    DragPoint()
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
        updateReturnPoints();
    }
    ~DragPoint() override
    {}
    void resized() override
    {
        sideLength = getParentHeight();
        setSize(sideLength, sideLength);
    }
    void paint(juce::Graphics& g) override
    {
        g.fillAll(setColor);
    }
    void updateReturnPoints()
    {
        leftX = getX();
        topY = getY();
        centerX = getX() + (getHeight() / 2);
        centerY = getY() + (getHeight() / 2);
        bottomY = getY() + getHeight();
        rightX = getX() + getHeight();
    }
    void mouseDown(const juce::MouseEvent &event) override
    {
        dragger.startDraggingComponent(this, event);
    }
    void mouseDrag(const juce::MouseEvent &event) override
    {
        dragger.dragComponent(this, event, &constrainer);
        updateReturnPoints();
    }
    void assignColor(juce::Colour inputColor)
    {
        setColor = inputColor;
    }
    int centerX, centerY, leftX, rightX, topY, bottomY;
private:
    bool firstDragLoop = true;
    int sideLength;
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    juce::Colour setColor = juce::Colours::blue;
};

class DraggerContainer : public juce::Component
{
public:
    enum side {top, right, bottom, left};
    
    const char* getSideName(side enumVal)
    {
        switch(enumVal)
        {
            case top:
                return "top";
            case right:
                return "right";
            case bottom:
                return "bottom";
            case left:
                return "left";
        }
    }
    
    DraggerContainer()
    {
        addAndMakeVisible(point);
        setSize(400, 300);
    }
    DraggerContainer(int xMinSet, int xMaxSet, int yMinSet, int yMaxSet)
    {
        addAndMakeVisible(point);
        contMinX = xMinSet;
        contMaxX = xMaxSet;
        contMinY = yMinSet;
        contMaxY = yMaxSet;
        contWidth = contMaxX - contMinX;
        contHeight = contMaxY - contMinY;
        setBounds(contMinX, contMinY, contWidth, contHeight);
        point.setBounds(contMinX, contMinY, contHeight, contHeight);
        point.setTopLeftPosition(0, 0);
    }
    ~DraggerContainer()
    {}
    void addPeer(DraggerContainer* peer, side limitSide)
    {
        peerSide = limitSide;
        peerCont = peer;
        switch(limitSide)
        {
            case top:
                maxYFromPeer = &peerCont->point.bottomY;
                lastMaxY = *maxYFromPeer;
            case right:
                maxXFromPeer = &peerCont->point.centerX;
                lastMaxX = *maxXFromPeer;
            case bottom:
                minYFromPeer = &peerCont->point.topY;
                lastMinY = *minYFromPeer;
            case left:
                minXFromPeer = &peerCont->point.centerX;
                lastMinX = *minXFromPeer;
        }
    }
    void updateValsFromPtr()
    {
       lastMaxY = *maxYFromPeer;
       lastMaxX = *maxXFromPeer;
       lastMinY = *minYFromPeer;
       lastMinX = *minXFromPeer;
    }
    
    void paint(juce::Graphics &g) override
    {g.fillAll(juce::Colours::white);}
    
    void checkLimitUpdates()
    {   int lastSetting;
        int* settingSource;
        switch(peerSide)
        {
            case top:
            {
                lastSetting = lastMaxY;
                settingSource = maxYFromPeer;
            }
            case right:
            {
                lastSetting = lastMaxX;
                settingSource = maxXFromPeer;
            }
            case bottom:
            {
                lastSetting = lastMinY;
                settingSource = minYFromPeer;
            }
            case left:
            {
                lastSetting = lastMinX;
                settingSource = minXFromPeer;
            }
        }
        if(lastSetting != *settingSource)//checks whether the limit has changed since the last update
        {
            //run something to reset the bounds here
            const char* limitSide = getSideName(peerSide);
            printf("limit on the %s changed to %d\n", limitSide, *settingSource);
            updateValsFromPtr(); //set the lastValues back to the pointer values
        }
    }
    void setChildColor(juce::Colour colorChoice)
    {
        point.assignColor(colorChoice);
    }
    // public data member(s)
    DragPoint point;
private:
    DraggerContainer* peerCont;
    side peerSide;
    int contWidth, contHeight, contMinX, contMaxX, contMinY, contMaxY; //all the dimension points
    int childX, childY; //top left corner of the DragPoint
    //these store data from another DragContainer about what limits are imposed on this DragContainer
    int* minXFromPeer;
    int* maxXFromPeer;
    int* minYFromPeer;
    int* maxYFromPeer;
    //these store the last saved limit from the peer and get checked against the pointers to see if anything has changed
    int lastMinX, lastMaxX, lastMinY, lastMaxY;
};




