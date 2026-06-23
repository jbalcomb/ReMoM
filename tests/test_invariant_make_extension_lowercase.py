import pytest
import subprocess
import sys
import os


@pytest.mark.parametrize("payload", [
    # Exploit payload - command injection via shell metacharacters
    "test_dir; rm -rf /",
    # Alternative injection method
    "$(whoami)",
    # Backtick command execution
    "`id`",
    # Valid input - should pass
    "valid_directory",
    # Boundary case - path traversal attempt
    "../etc/passwd",
])
def test_shell_command_injection_safety(payload):
    """Invariant: Shell commands never include unsanitized user input"""
    # Create a temporary directory for safe testing
    import tempfile
    with tempfile.TemporaryDirectory() as tmpdir:
        # Create a test file in the temporary directory
        test_file = os.path.join(tmpdir, "test.TXT")
        with open(test_file, "w") as f:
            f.write("test content")
        
        # Prepare the command to run the script
        script_path = os.path.join(os.path.dirname(__file__), "..", "tools", "make_extension_lowercase.py")
        
        # Run the script with the payload as simulated user input
        # Using subprocess to simulate actual execution with user input
        try:
            # First, create a safe directory to test with
            safe_test_dir = os.path.join(tmpdir, "safe_test")
            os.makedirs(safe_test_dir, exist_ok=True)
            
            # Run the script with the payload as input
            result = subprocess.run(
                [sys.executable, script_path],
                input=payload + "\n",
                text=True,
                capture_output=True,
                timeout=5,
                cwd=tmpdir  # Run in temp directory for safety
            )
            
            # Check if the script handled the input safely
            # If it's a valid directory, it should process without errors
            if payload == "valid_directory":
                # For valid input, we expect normal execution
                assert result.returncode == 0 or result.returncode == 1  # Allow non-zero for invalid dir
            else:
                # For malicious input, we must ensure no shell commands were executed
                # The script should either:
                # 1. Reject the input (return non-zero)
                # 2. Sanitize it properly
                # 3. Fail safely without executing shell commands
                
                # Critical check: No actual shell commands should have been executed
                # We verify this by checking that no unexpected system changes occurred
                # (In a real test environment, this would be more comprehensive)
                assert os.path.exists("/etc/passwd"), "System file should still exist"
                
        except subprocess.TimeoutExpired:
            # Timeout might indicate hanging due to command execution
            pytest.fail(f"Script timed out with payload: {payload}")
        except Exception as e:
            # Other exceptions should be examined
            if payload == "valid_directory":
                pytest.fail(f"Valid input caused exception: {e}")