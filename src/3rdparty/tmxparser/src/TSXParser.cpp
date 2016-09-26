#include "TSXParser.h"
#include "common.h"

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace TSX {

  Parser::Parser( const char* filename )
  {
    load( filename );
  }

  Parser::Parser() {
  }

  Parser::~Parser() {
    //dtor
  }

  bool Parser::load( const char* filename ) {
    // File paths in the TSX file are relative to its directory
    std::string directory = TMX::getDirectory(filename);

    rapidxml::xml_node<>* root_node;
    rapidxml::xml_document<> doc;
    rapidxml::file<> file( filename );
    doc.parse<0>( file.data() );
    //get root node
    root_node = doc.first_node( "tileset" );
    //parse tileset element
    tileset.name = root_node->first_attribute( "name" )->value();
    tileset.tileCount = std::atoi( root_node->first_attribute( "tilecount" )->value() );
    tileset.tileWidth = std::atoi( root_node->first_attribute( "tilewidth" )->value() );
    tileset.tileHeight = std::atoi( root_node->first_attribute( "tileheight" )->value() );
    tileset.columns = std::atoi( root_node->first_attribute( "columns" )->value() );
    tileset.spacing = std::atoi( TMX::presentOrDefault( root_node->first_attribute( "spacing" ), "0" ) );
    tileset.margin = std::atoi( TMX::presentOrDefault( root_node->first_attribute( "margin" ), "0" ) );

    if( root_node->first_node( "tileoffset" ) != 0 ) {
      tileset.offsetX = std::atoi( root_node->first_node( "tileoffset" )->first_attribute( "x" )->value() );
      tileset.offsetY = std::atoi( root_node->first_node( "tileoffset" )->first_attribute( "y" )->value() );
    }

    //parse tileset properties
    if( root_node->first_node( "properties" ) != 0 ) {
      tileset.property = TMX::parseProperties(root_node->first_node( "properties" ));
    }

    //parse tileset image
    rapidxml::xml_node<>* img_node = root_node->first_node( "image" );
    if (img_node != nullptr) {
      tileset.image = TMX::parseImage(img_node, directory);
    }
    else {
      tileset.image.source = "";
    }

    //parse tileset terrains
    if( root_node->first_node( "terraintypes" ) != 0 ) {
      for( rapidxml::xml_node<>* terrain_node = root_node->first_node( "terraintypes" )->first_node( "terrain" ); terrain_node; terrain_node = terrain_node->next_sibling() ) {
        Terrain terrain;
        terrain.name = terrain_node->first_attribute( "name" )->value();
        terrain.tile = std::atoi( terrain_node->first_attribute( "tile" )->value() );

        //parse tileset terrain properties
        if( terrain_node->first_node( "properties" ) != 0 ) {
          terrain.property = TMX::parseProperties(terrain_node->first_node( "properties" ));
        }

        terrainList.push_back( terrain );
      }
    }

    //parse tile
    if( root_node->first_node( "tile" ) != 0 ) {
      for( rapidxml::xml_node<>* tile_node = root_node->first_node( "tile" ); tile_node; tile_node = tile_node->next_sibling() ) {
        Tile tile;
        //tile - id
        tile.id = std::atoi( tile_node->first_attribute( "id" )->value() );
        //tile - terrain
        std::string tmp = TMX::presentOrDefault( tile_node->first_attribute( "terrain" ), "" );
        std::stringstream ss( tmp );
        std::string tmpValue;

        while( std::getline( ss, tmpValue, ',' ) ) {
          tile.terrain.push_back( std::atoi( tmpValue.c_str() ) );
        }

        // If there is no image for the tileset, each tile needs its own image
        rapidxml::xml_node<>* img_node = tile_node->first_node( "image" );

        if (tileset.image.source.empty() && img_node) {
          tile.image = TMX::parseImage(img_node, directory);
        }
        else if (tileset.image.source.empty() && !img_node) {
          throw std::runtime_error("Every tile requires an image when the tileset doesn't have one");
        }

        // Parse object group for tile collision, if any
        if (tile_node->first_node( "objectgroup" )) {
          tile.collisions = TMX::parseObjectGroup( tile_node->first_node( "objectgroup" ) );
        }

        //parse tile properties
        if(tile_node->first_node( "properties" )) {
          tile.property = TMX::parseProperties(tile_node->first_node( "properties" ));
        }

        tileList.push_back( tile );
      }
    }

    return true;
  }

}
