# miniproj/views.py

from django.shortcuts import render


def control_page(request):
    return render(request, "control.html")


from django.shortcuts import render, redirect

# Simulated patient data
patients = [
    {
        "id": 1,
        "name": "John Doe",
        "age": 29,
        "gender": "Male",
        "condition": "Rehabilitation after wrist injury",
        "patient_type": "Post-Surgery",
        "num_sets": 5,
        "sets_completed": 2,
        "last_updated": "2023-11-28",
    },
    {
        "id": 2,
        "name": "Jane Smith",
        "age": 34,
        "gender": "Female",
        "condition": "Rehabilitation after shoulder surgery",
        "patient_type": "Sports Injury",
        "num_sets": 7,
        "sets_completed": 4,
        "last_updated": "2023-11-28",
    },
]


def dashboard(request):
    # Get selected patient (default to the first patient)
    selected_patient_id = int(request.GET.get("patient_id", 1))
    selected_patient = next(
        (p for p in patients if p["id"] == selected_patient_id), patients[0]
    )

    # Thresholds based on patient type
    patient_type_thresholds = {
        "Post-Surgery": {
            "thumb": 80,
            "index": 75,
            "middle": 85,
            "ring": 70,
            "pinky": 60,
        },
        "Sports Injury": {
            "thumb": 110,
            "index": 100,
            "middle": 120,
            "ring": 95,
            "pinky": 80,
        },
    }
    thresholds = patient_type_thresholds.get(
        selected_patient["patient_type"],
        patient_type_thresholds["Post-Surgery"],
    )

    # Notification messages
    notification = None
    motor_status = "Stopped"

    if request.method == "POST":
        # Update sets
        if "update_sets" in request.POST:
            selected_patient["num_sets"] = int(
                request.POST.get("num_sets", selected_patient["num_sets"])
            )
            selected_patient["last_updated"] = "2023-11-29"
            notification = f"Updated sets for {selected_patient['name']} to {selected_patient['num_sets']}."
        # Start motor
        elif "start_motor" in request.POST:
            motor_status = "Running"
            notification = f"Motor started for {selected_patient['name']}."
        # Stop motor
        elif "stop_motor" in request.POST:
            motor_status = "Stopped"
            notification = f"Motor stopped for {selected_patient['name']}."

    return render(
        request,
        "dashboard.html",
        {
            "patients": patients,
            "selected_patient": selected_patient,
            "thresholds": thresholds,
            "notification": notification,
            "motor_status": motor_status,
        },
    )
