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

class DraggerContainer : public juce::Component,
public juce::ComponentListener
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
    DraggerContainer(int xMinSet, int yMinSet, int widthSet, int heightSet)
    {
        addAndMakeVisible(point);
        contMinX = xMinSet;
        contMaxX = xMinSet + widthSet;
        contMinY = yMinSet;
        contMaxY = yMinSet + heightSet;
        contWidth = widthSet;
        contHeight = heightSet;
        setBounds(contMinX, contMinY, contWidth, contHeight);
        point.setBounds(contMinX, contMinY, contHeight, contHeight);
        point.setTopLeftPosition(0, 0);
      
    }
    ~DraggerContainer()
    {}
    void addPeer1(DraggerContainer* peer, side limitSide)
    {
        peerSide1 = limitSide;
        peerCont1 = peer;
        switch(limitSide)
        {
            case top:
                maxYFromPeer1 = &peerCont1->point.bottomY;
                lastMaxY1 = *maxYFromPeer1;
            case right:
                maxXFromPeer1 = &peerCont1->point.leftX;
                lastMaxX1 = *maxXFromPeer1;
            case bottom:
                minYFromPeer1 = &peerCont1->point.topY;
                lastMinY1 = *minYFromPeer1;
            case left:
                minXFromPeer1 = &peerCont1->point.rightX;
                lastMinX1 = *minXFromPeer1;
        }
        peerCont1->point.addComponentListener(this);
    }
    void updateValsFromPtr()
    {
        switch(peerSide1){
                case top:
                    lastMaxY1 = *maxYFromPeer1;
                case right:
                    lastMaxX1 = *maxXFromPeer1;
                case bottom:
                    lastMinY1 = *minYFromPeer1;
                case left:
                    lastMinX1 = *minXFromPeer1;
        }
       
    }
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override
    {
        printf("Peer DragPoint moved\n");
        checkLimitUpdates();
    }
    void paint(juce::Graphics &g) override
    {g.fillAll(juce::Colours::white);}
    
    void checkLimitUpdates()
    {   int lastSetting;
        int* settingSource;
        switch(peerSide1)
        {
            case top:
            {
                lastSetting = lastMaxY1;
                settingSource = maxYFromPeer1;
            }
            case right:
            {
                lastSetting = lastMaxX1;
                settingSource = maxXFromPeer1;
            }
            case bottom:
            {
                lastSetting = lastMinY1;
                settingSource = minYFromPeer1;
            }
            case left:
            {
                lastSetting = lastMinX1;
                settingSource = minXFromPeer1;
            }
        }
        if(lastSetting != *settingSource)//checks whether the limit has changed since the last update
        {
            const char* limitSide = getSideName(peerSide1);
            printf("limit on the %s changed to %d\n", limitSide, *settingSource);
            updateValsFromPtr(); //set the lastValues back to the pointer values
            switch(peerSide1)
            {
                case top:
                    lastSetting = *settingSource; //MaxY(), height gets limited
                    contMaxY = lastSetting;
                    contHeight = contMaxY - contMinY;
                    setBounds(contMinX, contMinY, contWidth, contHeight);
                case right:
                        lastSetting = *settingSource; //MaxX, width gets limited
                        contMaxX = lastSetting;
                        contWidth = contMaxX - contMinX;
                        setBounds(contMinX, contMinY, contWidth, contHeight);
                case bottom:
                    lastSetting = *settingSource; //MinY, height gets limited
                    contMinY = lastSetting;
                    contHeight = contMaxY - contMinY;
                    setBounds(contMinX, contMinY, contWidth, contHeight);
                case left:
                    lastSetting = *settingSource; //MinX, width gets limited
                    contMinX = lastSetting;
                    contWidth = contMaxX - contMinX;
                    setBounds(contMinX, contMinY, contWidth, contHeight);
            }
        }
    }
    void setChildColor(juce::Colour colorChoice)
    {
        point.assignColor(colorChoice);
    }
    // public data member(s)
    DragPoint point;
private:
    DraggerContainer* peerCont1;
    side peerSide1, peerSide2;
    int contWidth, contHeight, contMinX, contMaxX, contMinY, contMaxY; //all the dimension points
    int childX, childY; //top left corner of the DragPoint
    //these store data from another DragContainer about what limits are imposed on this DragContainer
    int* minXFromPeer1;
    int* maxXFromPeer1;
    int* minYFromPeer1;
    int* maxYFromPeer1;
    
    //these store the last saved limit from the peer and get checked against the pointers to see if anything has changed
    int lastMinX1, lastMaxX1, lastMinY1, lastMaxY1;
    int lastMinX2, lastMaxX2, lastMinY2, lastMaxY2;
};


class peerConstraint
{
    
};
