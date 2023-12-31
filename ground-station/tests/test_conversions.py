# Test cases for conversion functions
__author__ = "Matteo Golin"

# Imports
import pytest
import modules.misc.converter as conv


# Tests
def test_celsius_to_fahrenheit() -> None:
    """Test that Celsius is properly converted to Fahrenheit."""
    assert conv.celsius_to_fahrenheit(12.0) == pytest.approx(53.6)


def test_metres_to_feet() -> None:
    """Test that metres are properly converted to feet."""
    assert conv.metres_to_feet(5.0) == pytest.approx(16.4)
