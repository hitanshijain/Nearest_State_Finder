# Nearest State Finder
This is a semester project for BU EC504 2022, all copyrights reserved.
## How to use?

Copy files to your computer then complie and run it. After software outputs text, input the location (Latitude, Longitude)
Then software would do automatic calculation and output nearest counties and their distances in descending order.

## How do we implement this?

We use two two methods to handle domestic locations and oversea locations. Geohash is used to find nearest counties of local locations, detailed method is in our presentation slides. R-B tree method is for oversea locations including locations far from North America like Hawaii and GUam. 

Presentation link: https://docs.google.com/presentation/d/1p4IA2yMZIYUiWuGP25p6Zor3CB1QIDxZkvYplDFNk48/edit?usp=sharing

## Advantages

Our software can find nearest counties in a very fast speed (Avg 2.4 ms) no matter what location you input. For oversea locations, speed would be a little slow, but it is still under 15 ms, which is significantly faster than other algorithms.
