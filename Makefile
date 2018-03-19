VERSION := $(shell node -e "console.log(require('./package.json').version)")

.PHONY: default release

# Add a default task so we don't release just because someone ran 'make'
default:
	@echo "Did you mean to release a new version?"
	@echo "If so, run 'make release'."

release:
	@echo "Creating release commit"
	@git commit -a -m "Release version $(VERSION)"

	@echo "Tagging release $(VERSION)"
	@git tag -m "$(VERSION)" v$(VERSION)

	@echo "Pushing commit and tags to GitHub"
	@git push
	@git push --tags

	@echo "Publishing to NPM"
	@echo 'Please `npm publish` after the CI servers finish building their binaries'
