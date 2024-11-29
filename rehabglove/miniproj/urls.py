# miniproj/urls.py

from django.urls import path
from . import views

urlpatterns = [
    path("control/", views.control_page, name="control_page"),
    path("dashboard/", views.dashboard, name="dashboard"),
]
