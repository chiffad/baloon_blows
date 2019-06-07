import QtQuick 2.11
import QtQml 2.11
import QtQuick.Window 2.11

Window
{
    visible: true
    width: Game.screen_width
    height: Game.screen_height

    Repeater
    {
      model: Game

      Rectangle
      {
        width: el_radius*2
        height: el_radius*2
        radius: el_radius
        color: "red"
        border.color: "black"
        border.width: 1

        x: x_pos
        y: y_pos

        MouseArea
        {
          anchors.fill: parent
          onClicked: Game.baloon_clicked(index);
        }
      }
    }

    Text
    {
        text: Game.ran_away_count
        anchors.top: parent.top
        anchors.left: parent.left
    }
    Text
    {
        text: Game.clicked_count
        anchors.top: parent.top
        anchors.right: parent.right
    }
    Text
    {
        id: game_finished
        anchors.centerIn: parent
        text: "Game finished!"
        visible: false
    }
    
    Connections
    {
      target: Game
      onFinished: game_finished.visible = true;
    }
}
