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
        bottomY = getBottom();
        rightX = getRight();
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


class DraggerContainer : public juce::Component,
public juce::ComponentListener
{
public:
    
    
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
    
    //this gets a third argument for index
    void addPeer(DraggerContainer* peer, side limitSide)
    {
        limitingPoints.push_back(peerPoint(peer, limitSide));
        peer->point.addComponentListener(this);
    }
    void componentMovedOrResized(juce::Component& component, bool wasMoved, bool wasResized) override
    {
        printf("Peer DragPoint moved\n");
        checkLimitUpdates();
    }
    void paint(juce::Graphics &g) override
    {g.fillAll(juce::Colours::white);}
    
    void checkLimitUpdates()
    {
        for(int i = 0; i < limitingPoints.size(); ++i)
        {
            auto fromSide = limitingPoints[i].sourceSide;
            int lastDimSetting;
            int* constrainedDim;
            switch(fromSide)
            {
                case top: // minY & height are limited
                    constrainedDim = limitingPoints[i].minYSource;
                    lastDimSetting = limitingPoints[i].lastMinY;
                    break;
                case right: //maxX
                    constrainedDim = limitingPoints[i].maxXSource;
                    lastDimSetting = limitingPoints[i].lastMaxX;
                    break;
                case bottom:
                    constrainedDim = limitingPoints[i].maxYSource;
                    lastDimSetting = limitingPoints[i].lastMaxY;
                    break;
                case left:
                    constrainedDim = limitingPoints[i].minXSource;
                    lastDimSetting = limitingPoints[i].lastMinX;
                    break;
            }
            if(lastDimSetting != *constrainedDim)
            {
                int newPos = *constrainedDim;
                int xMinSet = point.getX();
                int yMinSet = point.getY();
                int xMaxSet = point.getRight();
                int yMaxSet = point.getBottom();
                int widthSet = point.getWidth();
                int heightSet = point.getHeight();
                switch(fromSide)
                {
                    case top:
                        yMinSet = newPos;
                        heightSet = yMaxSet - yMinSet;
                        break;
                    case right:
                        xMaxSet = newPos;
                        widthSet = xMaxSet - xMinSet;
                        break;
                    case bottom:
                        yMaxSet = newPos;
                        heightSet = yMaxSet - yMinSet;
                        break;
                    case left:
                        xMinSet = newPos;
                        widthSet = xMaxSet - xMinSet;
                        break;
                }
                setBounds(xMinSet, yMinSet, widthSet, heightSet);
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
    class peerPoint
    {
        public:
        peerPoint(DraggerContainer* source, side onSide)
        {
            sourceContainer = source;
            sourceSide = onSide;
            minXSource = &sourceContainer->point.leftX;
            minYSource = &sourceContainer->point.topY;
            maxXSource = &sourceContainer->point.rightX;
            maxYSource = &sourceContainer->point.bottomY;
        }
        ~peerPoint() {}
        void updateLastPoints()
        {
            lastMinX = *minXSource;
            lastMinY = *minYSource;
            lastMaxX = *maxXSource;
            lastMaxY = *maxYSource;
        }
        //data members
        DraggerContainer* sourceContainer;
        int* minXSource;
        int* maxXSource;
        int* minYSource;
        int* maxYSource;
        int lastMinX, lastMaxX, lastMinY, lastMaxY;
        side sourceSide;
    };
    std::vector<peerPoint> limitingPoints;
    //these store the last saved limit from the peer and get checked against the pointers to see if anything has changed
    int lastMinX1, lastMaxX1, lastMinY1, lastMaxY1;
    int lastMinX2, lastMaxX2, lastMinY2, lastMaxY2;
};


