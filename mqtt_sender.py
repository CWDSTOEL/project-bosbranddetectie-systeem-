import paho.mqtt.client as mqtt
import time

broker = "localhost"
port = 1883
topic = "rook/melding"

client = mqtt.Client()
client.connect(broker, port, 60)

while True:
    invoer = input("Typ 'rook' om een melding te sturen, of 'stop' om te stoppen: ")
    if invoer == "rook":
        client.publish(topic, "Rook gedetecteerd!")
        print("Bericht verstuurd!")
    elif invoer == "stop":
        break
    else:
        print("Onbekende invoer.")
