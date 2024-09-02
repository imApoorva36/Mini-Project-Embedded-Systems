from django.shortcuts import render
from .forms import XYZForm


def index(request):
    if request.method == "POST":
        form = XYZForm(request.POST)
        if form.is_valid():
            # Handle the form data here if needed
            pass
    else:
        form = XYZForm()

    return render(request, "index.html", {"form": form})
