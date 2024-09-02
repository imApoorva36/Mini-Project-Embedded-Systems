from django import forms


class XYZForm(forms.Form):
    x = forms.FloatField(label="X Value")
    y = forms.FloatField(label="Y Value")
    z = forms.FloatField(label="Z Value")
