import paho.mqtt.client as mqtt
import mysql.connector
import json

def on_connect(client, userdata, flags, rc):
    print("Verbonden met MQTT")
    client.subscribe("rook/melding")

def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    print("Ontvangen data:", data)

    db = mysql.connector.connect(
        host="localhost",
        user="root",
        password="",  # Laat leeg als je geen wachtwoord hebt
        database="sensor_data"
    )
    cursor = db.cursor()
    cursor.execute("INSERT INTO metingen (temperatuur, luchtvochtigheid, rook, brand) VALUES (%s, %s, %s, %s)",
                   (data["temperature"], data["humidity"], data["smoke"], data["fire"]))
    db.commit()
    db.close()

client = mqtt.Client()
client.username_pw_set("arduino", "abc123")
client.on_connect = on_connect
client.on_message = on_message

client.connect("145.93.236.59", 1883, 60)
client.loop_forever()
