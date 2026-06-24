from moyi_edge import MoYiPipeline


def main() -> None:
    pipeline = MoYiPipeline()
    result = pipeline.process_text("Dung may lai, kiem tra cam bien an toan.")
    print(result)


if __name__ == "__main__":
    main()
