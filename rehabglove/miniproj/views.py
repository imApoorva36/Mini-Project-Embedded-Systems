from django.shortcuts import render
import json
from django.http import JsonResponse


def control_view(request):
    if request.method == "POST":
        # Extract form data for target and assisted angles
        target_angles = [
            request.POST.get(f"targetAngle{i}", 0) for i in range(5)
        ]
        assisted_angles = [
            request.POST.get(f"assistedAngle{i}", 0) for i in range(5)
        ]

        # Send data to WebSocket (this assumes you have the WebSocket setup on the frontend)
        data = {"targetAngle": target_angles, "assistedAngle": assisted_angles}

        # JSON response to be sent to WebSocket
        return JsonResponse(data)

    return render(request, "control.html")
