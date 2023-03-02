/*
Copyright (C) 2022 Paolo Gaboardi

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3.0 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QQmlContext>
#include <QDir>

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QString Latitude, QString Longitude, QString Latitude1, QString Longitude1,
                       bool MappaAuto, qint16 AngI, qint16 AngF, quint16 Direzione, quint16 raggio, float IncNord, float IncEst,
                       QString GPS_Np0, QString GPS_Ep0, QStringList ListaGPS_Np0, QStringList ListaGPS_Ep0, QStringList ListaKML, QWidget *parent = 0);
    ~MapWidget();

//    Q_PROPERTY(QString fileHTML MEMBER FileHTML NOTIFY FileHTMLChanged)
    Q_PROPERTY(QString percorsoMap MEMBER PercorsoMap NOTIFY FileHTMLChanged)

signals:
    void FileHTMLChanged();
    void ReloadMap();
    void EraseMap();
    void TurnOffMap(void);
    void TurnOnMap(void);

private:
    Ui::MapWidget *ui;

    QQmlContext* ContextMappa;

    QString FileHTML, PercorsoMap;
    QString FieldDirectory;
    QFile FileKML;

    QString FileHTMLModel="\n"
            "<!DOCTYPE html>\n"
            "<html>\n"

            "<head>\n"
            "<style>\n"
            "html, body, #gmap_canvas {\n"
                "width: 100%;\n"
                "height: 100%;\n"
                "margin: 0;\n"
                "padding: 0;\n"
            "}\n"
            "#map_canvas {\n"
                "position: relative;\n"
            "}\n"
            "</style>\n"
            "</head>\n"

            "<body>\n"

            "<script src='https://maps.googleapis.com/maps/api/js?key=API_KEY'></script>\n"

            "<div id=\"gmap_canvas\"></div>\n"

            "<script type='text/javascript'>\n"
            "function initMap(){\n"
                "var myOptions = {\n"
                    "zoom:17,\n"
                    "center:new google.maps.LatLng(44.917457,9.774474),\n"
                    "mapTypeId: google.maps.MapTypeId.SATELLITE,\n"
                    "streetViewControl: false\n"
                "};\n"
                "map = new google.maps.Map(document.getElementById('gmap_canvas'), myOptions);\n"
                "marker = new google.maps.Marker({map: map,position: new google.maps.LatLng(44.917457,9.774474)});\n"

                "var SprinklerPathCoordinates = [\n"
                    "{lat: 44.000000, lng: 9.000000},\n"
                    "{lat: 44.000001, lng: 9.000001}\n"
                "];\n"
                "var SprinklerPath = new google.maps.Polyline({\n"
                    "path: SprinklerPathCoordinates,\n"
                    "geodesic: true,\n"
                    "strokeColor: '#FFFF00',\n"
                    "strokeOpacity: 1.0,\n"
                    "strokeWeight: 3\n"
                "});\n"

                "SprinklerPath.setMap(null);\n"
            "}\n"
            "google.maps.event.addDomListener(window, 'load', initMap);\n"
            "</script>\n"

            "</body>\n"
            "</html>";

//    QString FileHTMLModel="\n"
//            "<!DOCTYPE html>\n"
//            "<html>\n"
//            "<body>\n"
//            "<div class=\"gmap_canvas\">\n"
//            "<iframe style=\"display:block; width:100%; height:100vh;\" id=\"gmap_canvas\" src=\"https://maps.google.com/maps?q=44.917457,9.774474&t=k&z=17&ie=UTF8&iwloc=&output=embed\" frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\">\n"
//            "</iframe>\n"
//            "</div>\n"
//            "</body>\n"
//            "</html>\n";

    QString FileHTMLModelA="\n"
            "<!DOCTYPE html>\n"
            "<html>\n"
            "\n"
            "<head>\n"
            "<style>\n"
            "html, body, #gmap_canvas {\n"
            "    width: 100%;\n"
            "    height: 100%;\n"
            "    margin: 0;\n"
            "    padding: 0;\n"
            "}\n"
            "#map_canvas {\n"
            "    position: relative;\n"
            "}\n"
            "</style>\n"
            "</head>\n"
            "    \n"
            "<body>    \n"
            " \n"
            "<script src='https://maps.googleapis.com/maps/api/js?key=API_KEY'></script>\n"
            "\n"
            "<div id=\"gmap_canvas\"></div>\n"
            "\n"
            "<script type='text/javascript'>\n"
            "\n"
            "var EarthRadiusMeters = 6378137.0; // meters\n"
            "/* Based the on the Latitude/longitude spherical geodesy formulae & scripts\n"
            "   at http://www.movable-type.co.uk/scripts/latlong.html\n"
            "   (c) Chris Veness 2002-2010\n"
            "*/ \n"
            "google.maps.LatLng.prototype.DestinationPoint = function (brng, dist) {\n"
            "var R = EarthRadiusMeters; // earth's mean radius in meters\n"
            "var brng = brng.toRad();\n"
            "var lat1 = this.lat().toRad(), lon1 = this.lng().toRad();\n"
            "var lat2 = Math.asin( Math.sin(lat1)*Math.cos(dist/R) + \n"
            "                      Math.cos(lat1)*Math.sin(dist/R)*Math.cos(brng) );\n"
            "var lon2 = lon1 + Math.atan2(Math.sin(brng)*Math.sin(dist/R)*Math.cos(lat1), \n"
            "                             Math.cos(dist/R)-Math.sin(lat1)*Math.sin(lat2));\n"
            "\n"
            "return new google.maps.LatLng(lat2.toDeg(), lon2.toDeg());\n"
            "}\n"
            "\n"
            "google.maps.LatLng.prototype.latRadians = function()\n"
            "{\n"
            "  return (Math.PI * this.lat()) / 180;\n"
            "}\n"
            "\n"
            "google.maps.LatLng.prototype.lngRadians = function()\n"
            "{\n"
            "  return (Math.PI * this.lng()) / 180;\n"
            "}\n"
            "\n"
            "google.maps.LatLng.prototype.distanceFrom = function(newLatLng) {\n"
            "  //var R = 6371; // km (change this constant to get miles)\n"
            "  var R = 6371000; // meters\n"
            "  var lat1 = this.lat();\n"
            "  var lon1 = this.lng();\n"
            "  var lat2 = newLatLng.lat();\n"
            "  var lon2 = newLatLng.lng();\n"
            "  var dLat = (lat2-lat1) * Math.PI / 180;\n"
            "  var dLon = (lon2-lon1) * Math.PI / 180;\n"
            "  var a = Math.sin(dLat/2) * Math.sin(dLat/2) +\n"
            "  Math.cos(lat1 * Math.PI / 180 ) * Math.cos(lat2 * Math.PI / 180 )\n"
            "  *\n"
            "  Math.sin(dLon/2) * Math.sin(dLon/2);\n"
            "  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));\n"
            "  var d = R * c;\n"
            "  return d;\n"
            "}\n"
            "\n"
            "// === A function which returns the bearing between two LatLng in radians ===\n"
            "// === If v1 is null, it returns the bearing between the first and last vertex ===\n"
            "// === If v1 is present but v2 is null, returns the bearing from v1 to the next vertex ===\n"
            "// === If either vertex is out of range, returns void ===\n"
            "google.maps.LatLng.prototype.Bearing = function(otherLatLng) {\n"
            "  var from = this;\n"
            "  var to = otherLatLng;\n"
            "  if (from.equals(to)) {\n"
            "    return 0;\n"
            "  }\n"
            "  var lat1 = from.latRadians();\n"
            "  var lon1 = from.lngRadians();\n"
            "  var lat2 = to.latRadians();\n"
            "  var lon2 = to.lngRadians();\n"
            "  var angle = - Math.atan2( Math.sin( lon1 - lon2 ) * Math.cos( lat2 ), Math.cos( lat1 ) * Math.sin( lat2 ) - Math.sin( lat1 ) * Math.cos( lat2 ) * Math.cos( lon1 - lon2 ) );\n"
            "  if ( angle < 0.0 ) angle  += Math.PI * 2.0;\n"
            "  if ( angle > Math.PI ) angle -= Math.PI * 2.0; \n"
            "  return parseFloat(angle.toDeg());\n"
            "}\n"
            "\n"
            "\n"
            "/**\n"
            " * Extend the Number object to convert degrees to radians\n"
            " *\n"
            " * @return {Number} Bearing in radians\n"
            " * @ignore\n"
            " */ \n"
            "Number.prototype.toRad = function () {\n"
            "  return this * Math.PI / 180;\n"
            "};\n"
            "\n"
            "/**\n"
            " * Extend the Number object to convert radians to degrees\n"
            " *\n"
            " * @return {Number} Bearing in degrees\n"
            " * @ignore\n"
            " */ \n"
            "Number.prototype.toDeg = function () {\n"
            "  return this * 180 / Math.PI;\n"
            "};\n"
            "\n"
            "/**\n"
            " * Normalize a heading in degrees to between 0 and +360\n"
            " *\n"
            " * @return {Number} Return \n"
            " * @ignore\n"
            " */ \n"
            "Number.prototype.toBrng = function () {\n"
            "  return (this.toDeg() + 360) % 360;\n"
            "};\n"
            "\n"
            "var infowindow = new google.maps.InfoWindow(\n"
            "  { \n"
            "    size: new google.maps.Size(150,50)\n"
            "  });\n"
            "\n"
            "\n"
            "\n"
            "function drawArc(center, initialBearing, finalBearing, radius) { \n"
            "var d2r = Math.PI / 180;   // degrees to radians \n"
            "var r2d = 180 / Math.PI;   // radians to degrees \n"
            "\n"
            "   var points = 32; \n"
            "\n"
            "   // find the raidus in lat/lon \n"
            "   var rlat = (radius / EarthRadiusMeters) * r2d; \n"
            "   var rlng = rlat / Math.cos(center.lat() * d2r); \n"
            "\n"
            "   var extp = new Array();\n"
            "\n"
            "   if (initialBearing > finalBearing) finalBearing += 360;\n"
            "   var deltaBearing = finalBearing - initialBearing;\n"
            "   deltaBearing = deltaBearing/points;\n"
            "   for (var i=0; (i < points+1); i++) \n"
            "   { \n"
            "      extp.push(center.DestinationPoint(initialBearing + i*deltaBearing, radius)); \n"
            "//      bounds.extend(extp[extp.length-1]);\n"
            "   } \n"
            "   return extp;\n"
            "   }\n"
            "\n"
            "\n"
            "//var bounds = null;\n"
            "\n"
            "function initMap(){\n"
            "\n"
            "    var startPoint = new google.maps.LatLng(44.917457, 9.774074);\n"
            "    var endPoint = new google.maps.LatLng(44.917457, 9.774874);  \n"
            "    var centerPoint = new google.maps.LatLng(44.917457, 9.774474);\n"
            "    var initPoint = new google.maps.LatLng(44.917057, 9.774174);\n"
            "    \n"
            "    var myOptions = {\n"
            "        zoom:17,\n"
            "        center: centerPoint,\n"
            "        mapTypeId: google.maps.MapTypeId.SATELLITE,\n"
            "        streetViewControl: false\n"
            "    };\n"
            "    map = new google.maps.Map(document.getElementById('gmap_canvas'), myOptions);\n"
            "    marker = new google.maps.Marker({map: map,position: centerPoint});\n"
            "\n"
            "    var SprinklerPathCoordinates = [\n"
            "        centerPoint,\n"
            "        initPoint\n"
            "    ];\n"
            "    var SprinklerPath = new google.maps.Polyline({\n"
            "        path: SprinklerPathCoordinates,\n"
            "        geodesic: true,\n"
            "        strokeColor: '#FFFF00',\n"
            "        strokeOpacity: 1.0,\n"
            "        strokeWeight: 3\n"
            "    });\n"
            "\n"
            "    SprinklerPath.setMap(null);\n"
            "    \n"
            "    //---------------------------------------------------------------------------------------------------------\n"
            " \n"
            "    var arcPts = drawArc(centerPoint, centerPoint.Bearing(startPoint), centerPoint.Bearing(endPoint), centerPoint.distanceFrom(startPoint));\n"
            "    // add the start and end lines\n"
            "    arcPts.push(centerPoint);\n"
            "//    bounds.extend(centerPoint);\n"
            "    arcPts.push(startPoint);\n"
            "  \n"
            "    var piePoly = new google.maps.Polygon({\n"
            "                   paths: [arcPts],\n"
            "                   strokeColor: \"#0000FF\",\n"
            "                   strokeOpacity: 0.5,\n"
            "                   strokeWeight: 2,\n"
            "                   fillColor: \"#00FFFF\",\n"
            "                   fillOpacity: 0.35,\n"
            "                   map: map\n"
            "       });\n"
            "   \n"
            "//    map.fitBounds(bounds);\n"
            "    \n"
            "    piePoly.setMap(map);\n"
            "\n"
            "//---------------------------------------------------------------------------------------------------------\n"
            "\n"
            "// Define the LatLng coordinates for the polygon's path.\n"
            "const BordoCoords = [\n"
            "{ lat: 0.00000000000000, lng: 0.000000000000000 },\n"
            "];\n"
            "// Construct the polygon.\n"
            "const BordoPoly = new google.maps.Polygon({\n"
            "  paths: BordoCoords,\n"
            "  strokeColor: \"#FF0000\",\n"
            "  strokeOpacity: 1,\n"
            "  strokeWeight: 2,\n"
            "  fillColor: \"#FF0000\",\n"
            "  fillOpacity: 0.1,\n"
            "});\n"
            "\n"
            "BordoPoly.setMap(null);\n"
            "\n"
            "}\n"
            "google.maps.event.addDomListener(window, 'load', initMap);\n"
            "</script>\n"
            "  \n"
            "</body>\n"
            "</html>";


    QString FileHTMLBlank="\n"
            "<!DOCTYPE html>\n"
            "<html>\n"
            "<head></head>\n"
            "<body></body>\n"
            "</html>";

};

#endif // MAPWIDGET_H
