import paho.mqtt.client as mqtt

def on_message(client, userdata, message):
    print(f"📥 Bericht ontvangen op topic '{message.topic}': {message.payload.decode()}")

client = mqtt.Client()
client.on_message = on_message
client.connect("localhost", 1883, 60)  # Of je lokale IP als je op een ander apparaat werkt
client.subscribe("rook/melding")

print("🔄 Wacht op berichten...")
client.loop_forever()
