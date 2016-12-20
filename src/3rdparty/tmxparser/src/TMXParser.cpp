#include "TMXParser.h"
#include "common.h"

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace TMX {

  Parser::Parser( const char* filename )
  {
    load( filename );
  }

  Parser::Parser()
  {
  }

  Parser::~Parser()
  {
  }

  bool Parser::load( const char* filename )
  {
    // File paths in the TMX file are relative to its directory
    std::string directory = getDirectory(filename);

    std::string version = VERSION;
    rapidxml::xml_node<>* root_node;
    rapidxml::xml_document<> doc;
    rapidxml::file<> file( filename );
    doc.parse<0>( file.data() );
    //get root nodes
    root_node = doc.first_node( "map" );

    //load map element
    if( root_node->first_attribute( "version" )->value() != version ) {
      std::cout << "ERROR: Map version mismatch. Required version: " << VERSION << "." << std::endl;
      return false;
    }

    mapInfo.version = root_node->first_attribute( "version" )->value();
    std::cout << "Version: " << mapInfo.version << std::endl;
    mapInfo.orientation = root_node->first_attribute( "orientation" )->value();
    std::cout << "Orientation: " << mapInfo.orientation << std::endl;
    mapInfo.width = std::atoi( root_node->first_attribute( "width" )->value() );
    std::cout << "Width: " << mapInfo.width << std::endl;
    mapInfo.height = std::atoi( root_node->first_attribute( "height" )->value() );
    std::cout << "Height: " << mapInfo.height << std::endl;
    mapInfo.tileWidth = std::atoi( root_node->first_attribute( "tilewidth" )->value() );
    std::cout << "Tile Width: " << mapInfo.tileWidth << std::endl;
    mapInfo.tileHeight = std::atoi( root_node->first_attribute( "tileheight" )->value() );
    std::cout << "Tile Height: " << mapInfo.tileHeight << std::endl;

    mapInfo.backgroundColor = presentOrDefault( root_node->first_attribute( "backgroundcolor" ), "#000000" );
    std::cout << "Background Color: " << mapInfo.backgroundColor << std::endl;

    if( root_node->first_node( "properties" ) != 0 ) {
      mapInfo.property = parseProperties(root_node->first_node( "properties" ));
    }

    std::cout << std::endl;

    for( rapidxml::xml_node<>* tileset_node = root_node->first_node( "tileset" ); tileset_node; tileset_node = tileset_node->next_sibling( "tileset" ) ) {
      Tileset tmpTileset;
      tmpTileset.firstGID = std::atoi( tileset_node->first_attribute( "firstgid" )->value() );
      tmpTileset.source = directory + findOrFail(tileset_node, "source" );

      std::cout << "Tileset[ First GID: " << tmpTileset.firstGID << " Source: " << tmpTileset.source << std::endl;
      tilesetList.push_back( tmpTileset );
    }

    for( rapidxml::xml_node<>* layer_node = root_node->first_node( "layer" ); layer_node; layer_node = layer_node->next_sibling( "layer" ) ) {
      TileLayer layer;
      layer.name = layer_node->first_attribute( "name" )->value();
      std::cout << std::endl << "Layer Name: " << layer.name << std::endl;

      if( layer_node->first_node( "properties" ) != 0 ) {
        layer.property = parseProperties(layer_node->first_node( "properties" ));
      }

      rapidxml::xml_node<>* data_node = layer_node->first_node( "data" );
      layer.data.encoding = data_node->first_attribute( "encoding" )->value();
      std::cout << "Layer Encoding: " << layer.data.encoding << std::endl;

      if( data_node->first_attribute( "compression" ) > 0 ) {
        layer.data.compression = data_node->first_attribute( "compression" )->value();
        std::cout << "Layer Compression: " << layer.data.compression << std::endl;
      }

      layer.data.contents = data_node->value();
      std::cout << "Layer Data: " << layer.data.contents << std::endl;
      tileLayer[layer.name] = layer;
    }

    for( rapidxml::xml_node<>* oGroup_node = root_node->first_node( "objectgroup" ); oGroup_node; oGroup_node = oGroup_node->next_sibling( "objectgroup" ) ) {

      ObjectGroup oGroup = parseObjectGroup(oGroup_node);

      objectGroup[oGroup.name] = oGroup;
    }

    for( rapidxml::xml_node<>* image_node = root_node->first_node( "imagelayer" ); image_node; image_node = image_node->next_sibling( "imagelayer" ) ) {
      ImageLayer imgLayer;
      std::cout << std::endl;
      imgLayer.name = image_node->first_attribute( "name" )->value();
      std::cout << "Image Layer Name: " << imgLayer.name << std::endl;

      imgLayer.opacity = std::atof( presentOrDefault( image_node->first_attribute( "opacity" ), "1.0" ) );
      std::cout << "Image Layer Opacity: " << imgLayer.opacity << std::endl;

      imgLayer.visible = std::atoi( presentOrDefault( image_node->first_attribute( "visible" ), "1" ) );
      std::cout << "Image Layer Visibility: " << imgLayer.visible << std::endl;

      imgLayer.offsetx = std::atof( presentOrDefault( image_node->first_attribute( "offsetx" ), "0" ) );
      std::cout << "Image Layer Offset X: " << imgLayer.offsetx << std::endl;

      imgLayer.offsety = std::atof( presentOrDefault( image_node->first_attribute( "offsety" ), "0" ) );
      std::cout << "Image Layer Offset Y: " << imgLayer.offsety << std::endl;

      if (image_node->first_node( "image" )) {
        imgLayer.image = parseImage(image_node->first_node( "image" ), directory);
      }

      if( image_node->first_node( "properties" ) != 0 ) {
        imgLayer.property = parseProperties(image_node->first_node( "properties" ));
      }

      imageLayer[imgLayer.name] = imgLayer;
    }

    return true;
  }

}
