<?xml version="1.0" encoding="UTF-8"?>
<tileset name="grass" tilewidth="128" tileheight="128" tilecount="5" columns="2">
 <tile id="0">
  <image width="128" height="128" source="../PNG/grass_earth_tiles/grass_earth_03.png"/>
 </tile>
 <tile id="1">
  <image width="128" height="128" source="../PNG/grass_earth_tiles/grass_earth_05.png"/>
  <objectgroup draworder="index"/>
 </tile>
 <tile id="2">
  <image width="128" height="128" source="../PNG/grass_earth_tiles/grass_earth_07.png"/>
  <objectgroup draworder="index"/>
 </tile>
 <tile id="3">
  <image width="128" height="128" source="../PNG/grass_earth_tiles/grass_earth_26.png"/>
 </tile>
 <tile id="4">
  <image width="128" height="128" source="../PNG/grass_earth_tiles/grass_earth_05.png"/>
  <animation>
   <frame tileid="4" duration="100"/>
   <frame tileid="3" duration="100"/>
  </animation>
 </tile>
</tileset>
